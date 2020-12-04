#ifndef ASIOAPPLICATION_CONNECTION_HPP
#define ASIOAPPLICATION_CONNECTION_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <Network.hpp>

#include "Context.hpp"
#include "Server.hpp"

/*
 * Represents one server-client connection
 */
class Connection
        : std::enable_shared_from_this<Connection>
{
public:
    using tcp = boost::asio::ip::tcp;
    using OwnerPtr = std::weak_ptr<Server>;

public:
    Connection(tcp::socket&& socket, Context& context, const OwnerPtr& owner);
    Connection(Connection&&) = default;

private:
    using Message       = Commons::Network::Message;
    using Task          = Commons::Network::Task;
    using TaskManager   = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;

    using Strand = boost::asio::io_context::strand;

    using ConnectionState = SslConnection::State;

    using sessionid_t = Server::sessionid_t;

private:
    void addTask(Task&&);
    void dispatchTask();

private:
    void onReceive(const Message&);
    void onSend();
    void onStateChange(ConnectionState);

    void onAnswerReceive(const Message&);
    void onRequestReceive(const Message&);

private:
    TaskManager mTaskManager;
    SslConnection mConnection;
    Context& mContext;
    Strand mStrand;
    std::weak_ptr<Server> mOwner;
    sessionid_t sessionid;

};


#endif //ASIOAPPLICATION_CONNECTION_HPP
