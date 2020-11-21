//
// Created by artem on 21.11.2020.
//

#include "Server.hpp"

Server::Server(boost::asio::io_context& ioContext, unsigned short port)
    : mIoContext(ioContext)
    , mAcceptor(ioContext, tcp::endpoint(tcp::v4(), port))
    , mSslContext(boost::asio::ssl::context::sslv23) // TODO: make Context-dependent
{

}

void Server::start() {

}

void Server::onAccept(const boost::system::error_code& ec)
{

}
