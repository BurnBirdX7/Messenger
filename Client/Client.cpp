#include "Client.hpp"
#include "Context.hpp"

Client::Client(Context& context)
    : mTaskManager()
    , mTaskManagerIdle(true)
    , mContext(context)
    , mConnection(nullptr)
    , mStrand(context.getIoContext())
    , mStateHandler(nullptr)
    , mNotificationHandler(nullptr)
    , mState(State::DISCONNECTED)
{
    // Connection setup
    boost::asio::ip::tcp::resolver resolver(mContext.getIoContext());
    auto endpoints = resolver.resolve(mContext.getServerAddress(), mContext.getServerPort());

    /*
    mConnection = std::make_shared<SslConnection>(mContext.getIoContext(),
                                                  mContext.getSslContext(),
                                                  SslConnection::HandshakeType::client,
                                                  endpoints);
    */

    mConnection.reset(new SslConnection( mContext.getIoContext(),
                                             mContext.getSslContext(),
                                             SslConnection::HandshakeType::client,
                                             endpoints ) );

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

void Client::connect()
{
    if (!mNotificationHandler && !mStateHandler)
        throw Error(ClientErrorCategory::HANDLERS_ARENT_SET, ClientCategory());
    else if (!mNotificationHandler)
        throw Error(ClientErrorCategory::NOTIFICATION_H_ISNT_SET, ClientCategory());
    else if (!mStateHandler)
        throw Error(ClientErrorCategory::STATE_H_ISNT_SET, ClientCategory());
    else if (mState != State::ABLE_TO_CONNECT)
        throw Error(ClientErrorCategory::UNABLE_TO_CONNECT, ClientCategory());

    Task helloTask = Task::createHelloTask([this] (Task::ErrorCode ec, ConstBuffer buffer) {
        if (ec == Task::ErrorCode::OK)
            changeState(State::CONNECTED);
        else
            std::cerr << "Can't connect" << std::endl;

    });
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

bool Client::isAbleToConnect() const
{
    return mState == State::ABLE_TO_CONNECT;
}

void Client::setNotificationHandler(const Client::NotificationHandler& handler)
{
    mNotificationHandler = handler;
}

void Client::setStateHandler(const Client::StateHandler& handler)
{
    mStateHandler = handler;
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
        if (mStateHandler)
            std::invoke(mStateHandler, state);
    }
}

void Client::addTask(Task&& task) {
    boost::asio::post(
            boost::asio::bind_executor(
                    mStrand,
                    [this, &task]() {
                        mTaskManager.addTask(std::move(task));

                        if (mTaskManagerIdle) {
                            mTaskManagerIdle = false;
                            dispatchTask();
                        }
                    }
            )
    );
}

void Client::dispatchTask() {

    assert(mTaskManagerIdle == false);

    boost::asio::post(
            boost::asio::bind_executor(
                    mStrand,
                    [this]() {
                        if (!mTaskManager.isQueueEmpty()) {
                            TaskManager::TaskId taskId = mTaskManager.dequeueTask();
                            mConnection->send(mTaskManager.makeMessageFromTask(taskId));
                            mTaskManager.releaseIfAnswer(taskId);
                        }
                        else {
                            mTaskManagerIdle = true;
                        }
                    }
            )
    );
}

void Client::onSend()
{
    // If SslConnection operation was completed and task manager is not empty we dispatch the next task
    if (!mTaskManager.isQueueEmpty())
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
        std::invoke(mNotificationHandler, purpose, message.getContentBuffer());
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::Priority::MEDIUM));
    }
    else {
        mTaskManager.addTask(Task(Purpose::DECLINED, task, Task::Priority::LOW));
    }

}

void Client::onStateChange(SslConnection::State state)
{
    switch (state) {
        case SslConnection::State::DISCONNECTED:
            changeState(State::DISCONNECTED);
            break;
        case SslConnection::State::CONNECTED:
            changeState(State::ABLE_TO_CONNECT);
            break;
    }
}
