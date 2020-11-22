//
// Created by artem on 21.11.2020.
//

#include "Connection.hpp"

Connection::Connection(tcp::socket&& socket, boost::asio::ssl::context& sslContext)
    : mConnection(SslConnection::makeServerSide(std::move(socket), sslContext))
    , mTaskManager()
{

    mConnection.addReceiveListener([this](const Message &message) {
        onReceive(message);
    });

    mConnection.setSendListener([this] (size_t /*bytes_transferred*/) {
        onSend();
    });
}

void Connection::dispatchTask()
{
    auto taskId = mTaskManager.dequeueTask();
    mConnection.send(mTaskManager.makeMessageFromTask(taskId));
}

void Connection::addTask(Connection::Task&& task)
{
    bool idleNow = mTaskManager.isEmpty();
    mTaskManager.addTask(std::move(task));

    if (idleNow)
        dispatchTask();
}

void Connection::onReceive(const Connection::Message& message)
{
    switch (message.header().purposeByte) {
        case Commons::Network::Purpose::ACCEPTED:
        case Commons::Network::Purpose::DECLINED:
            onAnswerReceive(message);
            break;
        default:
            onRequestReceive(message);
    }
}

void Connection::onSend()
{
    if (!mTaskManager.isEmpty())
        dispatchTask();
}

void Connection::onAnswerReceive(const Connection::Message& message)
{
    auto ec = message.header().purposeByte == Commons::Network::Purpose::ACCEPTED
            ? Task::OK
            : Task::DECLINED_BY_RECEIVER;

    mTaskManager.completeTask(message.header().taskId, ec, message.getContentBuffer());
}

void Connection::onRequestReceive(const Connection::Message& message)
{
    // TODO: Receive message precessing

    switch (message.header().purposeByte) {
        case Commons::Network::Purpose::SEND_CHAT_MSG:
        case Commons::Network::Purpose::LOGIN:
            Task task(Commons::Network::Purpose::ACCEPTED, message.header().taskId);
    }

}
