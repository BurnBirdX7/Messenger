#ifndef ASIOAPPLICATION_CLIENT_HPP
#define ASIOAPPLICATION_CLIENT_HPP

#include <memory>
#include <functional>

#include <boost/asio.hpp>

#include <Network.hpp>


class Client {
public:

    using tcp = boost::asio::ip::tcp;

    explicit Client(boost::asio::io_context& ioContext);

    void start();

    void sendMessage(uint32_t chatId, const std::string& message);

    void login(const std::string& login, const std::string& password);


private:

    using Task = Commons::Network::Task;
    using TaskManager = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;
    using ConstBuffer = Commons::Network::ConstBuffer;
    using Message = Commons::Network::Message;
    using Purpose = Commons::Network::Purpose;

    using IoContext = boost::asio::io_context;
    using Strand = IoContext::strand;
    using ConnectionPtr = std::shared_ptr<SslConnection>;

private:
    void addTask(Task&&);
    void dispatchTask();
    void onSend();
    void onReceive(const Message&);

    void onReceiveAnswer(const Message&);
    void onReceiveRequest(const Message&);

protected:
    IoContext& mIoContext;

private:
    TaskManager mTaskManager;
    ConnectionPtr mConnection;
    Strand mStrand;

    bool mIsAuthorised;

};


#endif //ASIOAPPLICATION_CLIENT_HPP
