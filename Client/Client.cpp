//
// Created by artem on 19.11.2020.
//

#include "Client.hpp"

Client::Client(boost::asio::io_context& ioContext) // TODO: make normal constructor
    : mTaskManager()
    , mIoContext(ioContext)
    , mConnection(nullptr)
    , mIsAuthorised(false)
{
    tcp::resolver resolver(ioContext);
    auto endpoints = resolver.resolve("127.0.0.1", "56666");

    boost::asio::ssl::context sslContext(boost::asio::ssl::context::sslv23);
    sslContext.load_verify_file("ca.pem"); // TODO: configurable verification file

    mConnection = std::make_shared<SslConnection>(ioContext,
                                                  sslContext,
                                                  boost::asio::ssl::stream_base::handshake_type::client,
                                                  endpoints);

    mConnection->addReceiveListener([this](const Message &message) {
        onReceive(message);
    });

    mConnection->setSendListener([this](size_t bytes_transferred) {
        onSend();
    });
}

void Client::start()
{
    Task helloTask = Task::createHelloTask(
            [](error_code_t ec, ConstBuffer /* buffer */)
            {
                if (ec != Task::OK)
                    throw std::runtime_error("Hello message was declined"); // TODO: replace with own exception
            });

    this->addTask(std::move(helloTask));
}


void Client::addTask(Task&& task)
{
    bool writeInProgress = mTaskManager.isEmpty();

    mTaskManager.addTask(std::move(task));

    if (!writeInProgress) {
        TaskManager::TaskId taskId = mTaskManager.dequeueTask();
        mConnection->send(mTaskManager.makeMessageFromTask(taskId));
    }
}

void Client::onReceive(const Client::Message& message) {
    TaskManager::TaskId taskId = message.header().taskId;

    Task::error_code_t ec;

    ec = (message.header().purposeByte == Commons::Network::Purpose::ACCEPTED)
            ? Task::OK
            : Task::DECLINED_BY_SERVER;

    mTaskManager.completeTask(taskId, ec, message.getContentBuffer());
}

void Client::login(const std::string& login, const std::string& password) {
    std::array<ConstBuffer, 2> seq = {boost::asio::buffer(login), boost::asio::buffer(password)};

    Task loginTask(Purpose::LOGIN,
                   seq,
                   [this](error_code_t ec, ConstBuffer)
                   {
                       if (!ec)
                           mIsAuthorised = true; // TODO: make Context-dependent
                   });

    this->addTask(std::move(loginTask));
}

void Client::sendMessage(uint32_t chatId, const std::string& message) {
    std::array<ConstBuffer, 2> seq = {boost::asio::buffer(&chatId, sizeof(chatId)), boost::asio::buffer(message)};

    Task msgTask(Purpose::SEND_CHAT_MSG,
                 seq,
                 [this](error_code_t ec, ConstBuffer) {
                     if (ec)
                         throw std::runtime_error("Message wasn't sent"); // TODO: make Context-dependent
                 });

}

void Client::onSend()
{
    // If SslConnection operation was completed and task manager is not empty we dispatch the next task
    if (!mTaskManager.isEmpty())
        dispatchTask();
}

void Client::dispatchTask()
{
    TaskManager::TaskId taskId = mTaskManager.dequeueTask();
    mConnection->send(mTaskManager.makeMessageFromTask(taskId));
}
