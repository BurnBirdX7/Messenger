#ifndef ASIOAPPLICATION_SERVER_HPP
#define ASIOAPPLICATION_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <Network.hpp>

class Server {
public:
    explicit Server(boost::asio::io_context&, unsigned short port);

    void start();

private:
    using tcp = boost::asio::ip::tcp;

private:
    using Message = Commons::Network::Message;

private:
    void onAccept(const boost::system::error_code& ec);

private:
    boost::asio::io_context& mIoContext;
    boost::asio::ssl::context mSslContext;
    tcp::acceptor mAcceptor;

};


#endif //ASIOAPPLICATION_SERVER_HPP
