#include "Client.hpp"

Client::Client(Context& context)
    : mTaskManager()
    , mContext(context)
    , mConnection(nullptr)
    , mIsAuthorised(false)
    , mStrand(context.getIoContext())
{
    tcp::resolver resolver(mContext.getIoContext());
    auto endpoints = resolver.resolve(mContext.getServerAddress(), mContext.getServerPort());

    mConnection = std::make_shared<SslConnection>(mContext.getIoContext(),
                                                  mContext.getSslContext(),
                                                  SslConnection::HandshakeType::client,
                                                  endpoints);

    mConnection->addReceiveListener(
            [this](const Message &message) {
                onReceive(message);
            }
    );

    mConnection->setSendListener(
            [this](size_t bytes_transferred) {
                onSend();
            }
    );
}

void Client::start()
{
    mConnection->start();

    Task helloTask = Task::createHelloTask(
            [](auto ec, ConstBuffer /* buffer */)
            {
                if (ec != Task::OK)
                    throw std::runtime_error("Hello message was declined"); // TODO: replace with own exception
            });

    addTask(std::move(helloTask));
}

// Creates buffer of std::string as a buffer of null-terminated string
boost::asio::const_buffer string_buffer(const std::string& str)
{
    return boost::asio::buffer(str.c_str(), str.length() + 1); // len + 1 to catch '\0'
}

void Client::login(const std::string& login, const std::string& password) {

    std::array<ConstBuffer, 2> seq = {string_buffer(login), string_buffer(password)};

    Task loginTask(Purpose::LOGIN,
                   seq,
                   [this](auto ec, ConstBuffer)
                   {
                       if (!ec)
                           // TODO: Authorize session
                           mIsAuthorised = true; // TODO: make BaseContext-dependent
                   });

    addTask(std::move(loginTask));
}

void Client::sendMessage(uint32_t chatId, const std::string& message) {
    std::array<ConstBuffer, 2> seq = {boost::asio::buffer(&chatId, sizeof(chatId)), boost::asio::buffer(message)};

    Task msgTask(Purpose::SEND_CHAT_MSG,
                 seq,
                 [this](auto ec, auto) {
                     if (ec)
                         throw std::runtime_error("Message wasn't sent"); // TODO: make BaseContext-dependent
                 });

}

void Client::onSend()
{
    // If SslConnection operation was completed and task manager is not empty we dispatch the next task
    if (!mTaskManager.isEmpty())
        dispatchTask();
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

void Client::onReceive(const Client::Message& message)
{
    switch(message.header().purposeByte) {
        case Commons::Network::Purpose::ACCEPTED:
        case Commons::Network::Purpose::DECLINED:
            onReceiveAnswer(message);
            break;
        default:
            onReceiveRequest(message);
            break;
    }
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

void Client::onReceiveAnswer(const Message& message)
{
    // TODO: On Receive: Answer
    auto ec = (message.header().purposeByte == Commons::Network::Purpose::ACCEPTED)
              ? Task::OK
              : Task::DECLINED_BY_RECEIVER;

    mTaskManager.completeTask(message.header().taskId, ec, message.getContentBuffer());
}

void Client::onReceiveRequest(const Message& message)
{
    // TODO: On Receive: Request
    // If HEARTBEAT message was received, we send ACCEPTED message immediately
    if (message.header().purposeByte == Commons::Network::Purpose::HEARTBEAT)
        mTaskManager.addTask(Task(Purpose::ACCEPTED, message.header().taskId, Task::HIGH));
}