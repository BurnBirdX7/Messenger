#include "Connection.hpp"

Connection::Connection(tcp::socket&& socket, Context& context, OwnerPtr owner)
    : mConnection(SslConnection::makeServerSide(std::move(socket), context.getSslContext()))
    , mTaskManager()
    , mTaskManagerIdle(true)
    , mContext(context)
    , mStrand(context.getIoContext())
    , mOwner(std::move(owner))
    , mSessionId(-1)
{
    mConnection.setReceiveListener([this](const Message &message) {
        onReceive(message);
    });

    mConnection.setSendListener([this](size_t /*bytes_transferred*/) {
        onSend();
    });

    mConnection.setStateListener([this](ConnectionState state) {
        onStateChange(state);
    });
}

void Connection::dispatchTask() {

    assert (mTaskManagerIdle == false);

    boost::asio::post(
            boost::asio::bind_executor(
                    mStrand,
                    [this]() {
                        if (!mTaskManager.isQueueEmpty()) {
                            auto taskId = mTaskManager.dequeueTask();
                            mConnection.send(mTaskManager.makeMessageFromTask(taskId));
                            mTaskManager.releaseIfAnswer(taskId);
                        }
                        else {
                            mTaskManagerIdle = true;
                        }
                    }
            )
    );
}

void Connection::addTask(Connection::Task&& task) {

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
    if (!mTaskManager.isQueueEmpty())
        dispatchTask();
}

void Connection::onStateChange(ConnectionState state)
{
    // Unauthorized connection on disconnect should just die
    // Authorized should notify it's owner

    switch(state) {
        case SslConnection::State::DISCONNECTED:
            mTaskManager.lock();
            mTaskManager.declineAllDispatched();
            // TODO: notify owner for an authorized connection
            break;
        case SslConnection::State::CONNECTED:
            mTaskManager.unlock();
            break;
    }

}

void Connection::onAnswerReceive(const Connection::Message& message)
{
    auto ec = message.header().purposeByte == Commons::Network::Purpose::ACCEPTED
            ? Task::OK
            : Task::DECLINED_BY_RECEIVER;

    mTaskManager.completeTask(message.getTaskId(), ec, message.getContentBuffer());
}

void Connection::onRequestReceive(const Connection::Message& message)
{
    // TODO: Receive message precessing

    auto& ctx = mContext;

    switch (message.header().purposeByte) {
        case Commons::Network::Purpose::SEND_CHAT_MSG:
        case Commons::Network::Purpose::LOGIN:
            Task task(Commons::Network::Purpose::ACCEPTED, message.getTaskId());
    }

}
