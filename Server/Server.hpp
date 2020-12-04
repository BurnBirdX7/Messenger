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

class Connection;

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
    using Acceptor = tcp::acceptor;
    using Message = Commons::Network::Message;
    using ConnectionPtr = std::shared_ptr<Connection>;

    using userid_t = int;
    using sessionid_t = int;
    using sessionhash_t = std::string;

private:
    void onAccept(const boost::system::error_code& ec, Socket s); // meet requirements of MoveAcceptHandler

    friend Connection;
    sessionid_t authorizeConnection(const ConnectionPtr& connection,
                                    const std::string& login,
                                    const std::string& password);

    sessionid_t authorizeConnection(const ConnectionPtr& connection,
                                    sessionid_t sessionId,
                                    sessionhash_t sessionHash);



private:
    Context& mContext;
    Acceptor mAcceptor;

    std::map<sessionid_t, ConnectionPtr> mAuthorizedConnections;

};


#endif //ASIOAPPLICATION_SERVER_HPP
