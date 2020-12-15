#include "Client.hpp"
#include "Context.hpp"

Client::Client(Context& context)
    : mTaskManager()
    , mContext(context)
    , mConnection(nullptr)
    , mStrand(context.getIoContext())
    , mStateHandler(std::nullopt)
    , mNotificationHandler(std::nullopt)
    , mState(State::DISCONNECTED)
{
    // Connection setup
    boost::asio::ip::tcp::resolver resolver(mContext.getIoContext());
    auto endpoints = resolver.resolve(mContext.getServerAddress(), mContext.getServerPort());

    mConnection = std::make_shared<SslConnection>(mContext.getIoContext(),
                                                  mContext.getSslContext(),
                                                  SslConnection::HandshakeType::client,
                                                  endpoints);

    mConnection->setReceiveListener([this](const Message& message) {
        onReceive(message);
    });

    mConnection->setSendListener([this](size_t bytes_transferred) {
        onSend();
    });

    mConnection->setStateListener([this](auto state) {
        onStateChange(state);
    });
}

void Client::start(const Task::CompletionHandler& handler)
{
    bool notifIsSet = mNotificationHandler.has_value();
    bool stateIsSet = mStateHandler.has_value();

    if (!notifIsSet && !stateIsSet)
        throw Error(ClientErrorCategory::HANDLERS_ARENT_SET, ClientCategory());
    else if (!notifIsSet)
        throw Error(ClientErrorCategory::NOTIFICATION_H_ISNT_SET, ClientCategory());
    else if (!stateIsSet)
        throw Error(ClientErrorCategory::STATE_H_ISNT_SET, ClientCategory());

    mConnection->start();

    Task helloTask = Task::createHelloTask(handler);

    addTask(std::move(helloTask));
}

Context& Client::getContext() const
{
    return mContext;
}

bool Client::isConnected() const
{
    return mState == State::AUTHORIZED || mState == State::CONNECTED;
}

bool Client::isAuthorized() const
{
    return mState == State::AUTHORIZED;
}

void Client::setNotificationHandler(const Client::NotificationHandler& handler)
{
    mNotificationHandler.emplace(handler);
}

void Client::setStateHandler(const Client::StateHandler& handler)
{
    mStateHandler.emplace(handler);
}

void Client::authorize(const std::string& login, const std::string& password, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(login)
        .append(password);

    auto my_handler = [=] (Task::ErrorCode ec, ConstBuffer buffer) {

        if (ec == Task::ErrorCode::OK)
            changeState(State::AUTHORIZED);

        if (handler)
            handler(ec, buffer);

    };

    Task task(Purpose::LOGIN, composer.getVector(), my_handler, Task::Priority::HIGH);
    addTask(std::move(task));
}

void Client::authorize(int sessionId, const std::string& hash, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(sessionId)
        .append(hash);

    auto my_handler = [=] (Task::ErrorCode ec, ConstBuffer buffer) {

        if (ec == Task::ErrorCode::OK)
            changeState(State::AUTHORIZED);

        if (handler)
            handler(ec, buffer);

    };

    Task task(Purpose::RESTORE_SESSION, composer.getVector(), my_handler, Task::Priority::HIGH);
    addTask(std::move(task));
}

void Client::deauthorize(const CompletionHandler& handler)
{
    auto my_handler = [=](Task::ErrorCode ec, ConstBuffer buffer) {
        if (ec == Task::ErrorCode::OK)
            _deauth();

        if (handler)
            handler(ec, buffer);

    };

    Task task(Purpose::LOGOFF, my_handler, Task::Priority::HIGH);
    addTask(std::move(task));
}


const std::string& Client::getBufferedMessage() const
{
    return mBufferedMessage;
}

const Client::NotificationHandler& Client::_n_handler() const
{
    return *mNotificationHandler;
}

const Client::StateHandler& Client::_s_handler() const
{
    return *mStateHandler;
}

void Client::_deauth()
{
    if (mState != State::DISCONNECTED) {
        changeState(State::CONNECTED);
        mTaskManager.declineAllPending();
        mTaskManager.declineAllDispatched();
    }
}

void Client::changeState(Client::State state)
{
    if (state != mState) {
        mState = state;
        std::invoke(_s_handler(), state);
    }
}

void Client::addTask(Task&& task) {
    boost::asio::post(
            boost::asio::bind_executor(
                    mStrand,
                    [this, &task]() {
                        bool idleNow = mTaskManager.isEmpty();

                        mTaskManager.addTask(std::move(task));

                        if (idleNow)
                            dispatchTask();
                    }
            )
    );
}

void Client::dispatchTask() {
    boost::asio::post(
            boost::asio::bind_executor(
                    mStrand,
                    [this]() {
                        TaskManager::TaskId taskId = mTaskManager.dequeueTask();
                        mConnection->send(mTaskManager.makeMessageFromTask(taskId));
                        mTaskManager.releaseIfAnswer(taskId);
                    }
            )
    );
}

void Client::onSend()
{
    // If SslConnection operation was completed and task manager is not empty we dispatch the next task
    if (!mTaskManager.isEmpty())
        dispatchTask();
}

void Client::onReceive(const Client::Message& message)
{
    switch(message.header().purposeByte) {
        case Purpose::ACCEPTED:
        case Purpose::DECLINED:
            onReceiveAnswer(message);
            break;
        default:
            onReceiveRequest(message);
            break;
    }
}

void Client::onReceiveAnswer(const Message& message)
{
    auto ec = (message.header().purposeByte == Purpose::ACCEPTED)
              ? Task::OK
              : Task::DECLINED_BY_RECEIVER;

    // Invokes listener
    mTaskManager.completeTask(message.header().taskId, ec, message.getContentBuffer());
}

void Client::onReceiveRequest(const Message& message)
{
    // If HEARTBEAT message was received, we send ACCEPTED message immediately
    auto purpose = message.getPurpose();
    auto task = message.getTaskId();

    if (purpose == Purpose::HEARTBEAT) {
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::Priority::HIGH));
    }
    else if (purpose == Purpose::DEAUTHORIZED) {
        Commons::Data::BufferDecomposer decomposer(message.getContentBuffer());
        mBufferedMessage = decomposer.get<std::string>();
        _deauth();
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::Priority::HIGH));
    }
    else if (Purpose::isNotification(purpose)) {
        std::invoke(_n_handler(), purpose, message.getContentBuffer());
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::Priority::MEDIUM));
    }
    else {
        mTaskManager.addTask(Task(Purpose::DECLINED, task, Task::Priority::LOW));
    }

}

void Client::onStateChange(SslConnection::State state)
{
    switch (state) {
        case SslConnection::State::CLOSED:
        case SslConnection::State::CLOSING:
        case SslConnection::State::IDLE:
        case SslConnection::State::TCP_IDLE:
        case SslConnection::State::SSL_IDLE:
            changeState(State::DISCONNECTED);
            break;
        case SslConnection::State::RUNNING:
            changeState(State::CONNECTED);
            break;
    }
}
