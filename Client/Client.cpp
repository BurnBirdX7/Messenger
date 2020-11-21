//
// Created by artem on 19.11.2020.
//

#include "Client.hpp"

Client::Client(boost::asio::io_context& ioContext) // TODO: make normal constructor
    : mTaskManager()
    , mIoContext(ioContext)
    , mConnection(nullptr) {
    tcp::resolver resolver(ioContext);
    auto endpoints = resolver.resolve("127.0.0.1", "56666");

    boost::asio::ssl::context sslContext(boost::asio::ssl::context::sslv23);
    sslContext.load_verify_file("ca.pem");

    mConnection = std::make_shared<SslConnection>(ioContext,
                                                  sslContext,
                                                  boost::asio::ssl::stream_base::handshake_type::client,
                                                  endpoints);

    mConnection->addListener([this](const Message &message) {
        incomingMessageListener(message);
    });
}

void Client::start() {
    Task task(Commons::Network::PurposeBytes::HELLO,
              [](error_code_t ec, ConstBuffer /* buffer */) {
                  if (ec != Task::OK)
                      throw std::runtime_error("Hello message was declined");
              },
              Task::HIGH);

    mTaskManager.addTask(task);

    doTask();
}

void Client::doTask()
{
    if (!mTaskManager.isEmpty()) {
        TaskManager::task_id_t taskId = mTaskManager.dequeueTask();
        mConnection->send(mTaskManager.makeMessageFromTask(taskId));
    }
}

void Client::incomingMessageListener(const Client::Message& message) {

    TaskManager::task_id_t taskId = message.header().taskId;

    Task::error_code_t ec;

    ec = (message.header().purposeByte == Commons::Network::PurposeBytes::ACCEPTED)
            ? Task::OK
            : Task::DECLINED_BY_SERVER;

    mTaskManager.completeTask(taskId, ec, message.getContentBuffer());

    doTask();
}
