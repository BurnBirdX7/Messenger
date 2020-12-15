#include "Server.hpp"
#include "Connection.hpp"

Server::Server(Context& context)
    : mContext(context)
    , mAcceptor(context.getIoContext(), tcp::endpoint(tcp::v4(), context.getPort()))
{
}

void Server::start()
{
    mAcceptor.async_accept(
            [this](const boost::system::error_code &ec, Socket socket) {
                onAccept(ec, std::move(socket));
            }
    );
}

void Server::onAccept(const boost::system::error_code& ec, Socket socket)
{
    mConnections[makeSessionId()] =
            std::make_shared<Connection>(std::move(socket), mContext, shared_from_this());

    mAcceptor.async_accept(
            [this](const boost::system::error_code &ec, Socket socket) {
                onAccept(ec, std::move(socket));
            }
    );
}

Server::sessionid_t Server::makeSessionId()
{
    // TODO: make query to DB
    return 0;
}
