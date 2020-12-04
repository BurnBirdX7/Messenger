#ifndef ASIOAPPLICATION_CLIENT_HPP
#define ASIOAPPLICATION_CLIENT_HPP

#include <memory>
#include <functional>

#include <boost/asio.hpp>

#include <Network.hpp>

#include "Context.hpp"

class Client {
public:

    using tcp = boost::asio::ip::tcp;

public:
    explicit Client(Context&);
    Client(Client&&) = default;

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete; // TODO: Make move-assignable

public:
    void start();

    void requestSendMessage(uint32_t chatId, const std::string& message);
    void requestLogin(const std::string& login, const std::string& password);

private:
    using Task          = Commons::Network::Task;
    using TaskManager   = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;
    using ConstBuffer   = Commons::Network::ConstBuffer;
    using Message       = Commons::Network::Message;
    using Purpose       = Commons::Network::Purpose;

    using IoContext     = boost::asio::io_context;
    using Strand        = IoContext::strand;
    using ConnectionPtr = std::shared_ptr<SslConnection>;

private:
    void addTask(Task&&);
    void dispatchTask();
    void onSend();
    void onReceive(const Message&);

    void onReceiveAnswer(const Message&);
    void onReceiveRequest(const Message&);

private:
    TaskManager mTaskManager;
    ConnectionPtr mConnection;
    Strand mStrand;
    Context& mContext;

    bool mIsAuthorised;

};


#endif //ASIOAPPLICATION_CLIENT_HPP
