#include "Connection.hpp"

Connection::Connection(tcp::socket&& socket, Context& context)
    : mConnection(SslConnection::makeServerSide(std::move(socket), context.getSslContext()))
    , mTaskManager()
    , mContext(context)
    , mStrand(context.getIoContext())
{
    mConnection.addReceiveListener([this](const Message &message) {
        onReceive(message);
    });

    mConnection.setSendListener([this] (size_t /*bytes_transferred*/) {
        onSend();
    });

    mConnection.start();
}

void Connection::dispatchTask() {
    boost::asio::post(
            boost::asio::bind_executor(
                    mStrand,
                    [this]() {
                        auto taskId = mTaskManager.dequeueTask();
                        mConnection.send(mTaskManager.makeMessageFromTask(taskId));
                        mTaskManager.releaseIfAnswer(taskId);
                    }
            )
    );
}

void Connection::addTask(Connection::Task&& task) {

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
