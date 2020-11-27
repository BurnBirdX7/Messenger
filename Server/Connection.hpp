#ifndef ASIOAPPLICATION_CONNECTION_HPP
#define ASIOAPPLICATION_CONNECTION_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <Network.hpp>

class Connection
        : std::enable_shared_from_this<Connection>
{
public:
    using tcp = boost::asio::ip::tcp;
    using IoContext = boost::asio::io_context;
    using SslContext = boost::asio::ssl::context;

public:
    Connection(tcp::socket&&, IoContext&, SslContext&); // TODO: Replace with application context

private:
    using Message = Commons::Network::Message;
    using Task = Commons::Network::Task;
    using TaskManager = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;

    using Strand = IoContext::strand;

private:
    void addTask(Task&&);
    void onReceive(const Message&);
    void onSend();
    void dispatchTask();

    void onAnswerReceive(const Message&);
    void onRequestReceive(const Message&);

private:
    TaskManager mTaskManager;
    SslConnection mConnection;
    IoContext& mIoContext; // TODO: Replace with application context
    Strand mStrand;


};


#endif //ASIOAPPLICATION_CONNECTION_HPP
