//
// Created by artem on 21.11.2020.
//

#include "Connection.hpp"

Connection::Connection(tcp::socket&& socket, boost::asio::ssl::context& sslContext)
    : mConnection(SslConnection::makeServerSide(std::move(socket), sslContext))
    , mTaskManager()
{}
