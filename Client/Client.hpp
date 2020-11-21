#ifndef ASIOAPPLICATION_CLIENT_HPP
#define ASIOAPPLICATION_CLIENT_HPP

#include <memory>
#include <functional>

#include <boost/asio.hpp>

#include <Network/TaskManager.hpp>
#include <Network/SslConnection.hpp>
#include <Network/Message.hpp>


class Client {
public:

    using tcp = boost::asio::ip::tcp;

    explicit Client(boost::asio::io_context& ioContext);

    void start();


private:

    using Task = Commons::Network::Task;
    using TaskManager = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;
    using error_code_t = Commons::Network::Task::error_code_t;
    using ConstBuffer = Commons::Network::ConstBuffer;
    using Message = Commons::Network::Message;

private:
    void doTask();

    void incomingMessageListener(const Message&);

private:

    TaskManager mTaskManager;
    boost::asio::io_context& mIoContext;
    std::shared_ptr<SslConnection> mConnection;

};


#endif //ASIOAPPLICATION_CLIENT_HPP
