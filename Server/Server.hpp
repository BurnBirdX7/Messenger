#ifndef ASIOAPPLICATION_SERVER_HPP
#define ASIOAPPLICATION_SERVER_HPP

#include <memory>
#include <map>
#include <set>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <Network.hpp>

#include "Context.hpp"
#include "Connection.hpp"

class Server
    : public std::enable_shared_from_this<Server>
{
public:
    explicit Server(Context& context);

    void start();

    // TODO: Connection authorization

private:
    using tcp = boost::asio::ip::tcp;
    using Socket = tcp::socket;
    using Message = Commons::Network::Message;
    using ConnectionPtr = std::shared_ptr<Connection>;

    using userid_t = int;
    using sessionid_t = int;

private:
    void onAccept(const boost::system::error_code& ec, Socket s); // meet requirements of MoveAcceptHandler

private:
    Context& mContext;
    tcp::acceptor mAcceptor;

    std::map<sessionid_t, ConnectionPtr> mAuthorizedConnections;

};


#endif //ASIOAPPLICATION_SERVER_HPP
