#include "Server.hpp"

Server::Server(Context& context)
    : mContext(context)
    , mAcceptor(context.getIoContext(), tcp::endpoint(tcp::v4(), context.getPort()))
{
    context.setServerPtr(shared_from_this());
}

void Server::start() {
    mAcceptor.async_accept(
            [this](const boost::system::error_code &ec, Socket socket) {
                onAccept(ec, std::move(socket));
            }
    );
}

void Server::onAccept(const boost::system::error_code& ec, Socket socket)
{
    Connection(std::move(socket), mContext);
}
