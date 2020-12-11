#include "Client.hpp"
#include "Context.hpp"

Client::Client(Context& context)
    : mTaskManager()
    , mContext(context)
    , mConnection(nullptr)
    , mStrand(context.getIoContext())
    , mDeauthorizationHandler(std::nullopt)
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
    bool deauthIsSet = mDeauthorizationHandler.has_value();
    bool notifIsSet = mNotificationHandler.has_value();

    if (!notifIsSet && !deauthIsSet)
        throw Error(ClientErrorCategory::HANDLERS_ARENT_SET, ClientCategory());
    else if (!notifIsSet)
        throw Error(ClientErrorCategory::NOTIFICATION_H_ISNT_SET, ClientCategory());
    else if (!deauthIsSet)
        throw Error(ClientErrorCategory::DEAUTHORIZATION_H_ISNT_SET, ClientCategory());

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

void Client::setDeauthorizationHandler(const Client::DeauthorizationHandler& handler)
{
    mDeauthorizationHandler.emplace(handler);
}

void Client::setNotificationHandler(const Client::NotificationHandler& handler)
{
    mNotificationHandler.emplace(handler);
}

void Client::setStateHandler(const Client::StateHandler& handler)
{
    mStateHandler.emplace(handler);
}

void Client::authorize()
{
    changeState(State::AUTHORIZED);
}

void Client::deauthorize()
{
    // Connection deauthorized but not disconnected
    changeState(State::CONNECTED);
    mTaskManager.declineAllPending();
    mTaskManager.declineAllDispatched();
}

const Client::DeauthorizationHandler& Client::_d_handler() const
{
    return *mDeauthorizationHandler;
}

const Client::NotificationHandler& Client::_n_handler() const
{
    return *mNotificationHandler;
}

const Client::StateHandler& Client::_s_handler() const
{
    return *mStateHandler;
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
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::HIGH));
    }
    else if (purpose == Purpose::DEAUTHORIZED) {
        Commons::Data::BufferDecomposer decomposer(message.getContentBuffer());
        std::invoke(_d_handler(), decomposer.get<std::string>());
        deauthorize();
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::HIGH));
    }
    else if (Purpose::isNotification(purpose)) {
        std::invoke(_n_handler(), purpose, message.getContentBuffer());
        mTaskManager.addTask(Task(Purpose::ACCEPTED, task, Task::MEDIUM));
    }
    else {
        mTaskManager.addTask(Task(Purpose::DECLINED, task, Task::LOW));
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
