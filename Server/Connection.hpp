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

public:
    explicit Connection(tcp::socket&&, boost::asio::ssl::context&);

private:
    using Task = Commons::Network::Task;
    using TaskManager = Commons::Network::TaskManager;

    using SslConnection = Commons::Network::SslConnection;

private:

    TaskManager mTaskManager;
    SslConnection mConnection;

};


#endif //ASIOAPPLICATION_CONNECTION_HPP
