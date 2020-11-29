#ifndef ASIOAPPLICATION_CONNECTION_HPP
#define ASIOAPPLICATION_CONNECTION_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <Network.hpp>

#include "Context.hpp"

/*
 * Represents one server-client connection
 */
class Connection
        : std::enable_shared_from_this<Connection>
{
public:
    using tcp = boost::asio::ip::tcp;

public:
    Connection(tcp::socket&&, Context&);

private:
    using Message = Commons::Network::Message;
    using Task = Commons::Network::Task;
    using TaskManager = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;

    using Strand = boost::asio::io_context::strand;

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
    Context& mContext;
    Strand mStrand;


};


#endif //ASIOAPPLICATION_CONNECTION_HPP
