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
    mConnections[makeSessionId()]
        .reset(new Connection(std::move(socket),mContext, shared_from_this()));

    start();
}

Server::sessionid_t Server::makeSessionId()
{
    // TODO: make query to DB
    static int id = 0;
    return id++;
}
