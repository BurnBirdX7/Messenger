#ifndef ASIOAPPLICATION_CLIENT_HPP
#define ASIOAPPLICATION_CLIENT_HPP

#include <memory>
#include <functional>
#include <vector>

#include <boost/asio.hpp>

#include <Network.hpp>
#include <Data/Buffer.hpp>
#include <Data/BufferComposer.hpp>
#include <Data/BufferDecomposer.hpp>

#include "ClientErrorCategory.hpp"

class Context;

class Client {
public:
    using Task          = Commons::Network::Task;
    using TaskManager   = Commons::Network::TaskManager;
    using SslConnection = Commons::Network::SslConnection;
    using ConstBuffer   = Commons::Network::ConstBuffer;
    using Message       = Commons::Network::Message;
    using Purpose       = Commons::Network::Purpose;

    using IoContext     = boost::asio::io_context;
    using Strand        = IoContext::strand;
    using ConnectionPtr = std::shared_ptr<SslConnection>;

    enum class State {
        DISCONNECTED,
        CONNECTED,
        AUTHORIZED
    };

    using CompletionHandler = Task::CompletionHandler;
    using NotificationHandler = std::function<void (uint8_t header, ConstBuffer content)>;
    using DeauthorizationHandler = std::function<void (const std::string& reason)>;
    using StateHandler = std::function<void (State)>;


public:
    explicit Client(Context&);
    Client(Client&&) = default;

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete;

public:
    // Sends HELLO message to server and starts work
    // Throws if handlers aren't set
    void start(const CompletionHandler& handler);

    // Returns reference to the associated Context object
    Context& getContext() const;
    bool isConnected() const;
    bool isAuthorized() const;

    void setDeauthorizationHandler(const DeauthorizationHandler&);
    void setNotificationHandler(const NotificationHandler&);
    void setStateHandler(const StateHandler&);

private:
    // Declares connection as authorized
    void authorize();

    // Declares connection as not authorized
    // ! Declines ALL tasks (pending and already dispatched)
    void deauthorize();

    const DeauthorizationHandler& _d_handler() const;
    const NotificationHandler&    _n_handler() const;
    const StateHandler&           _s_handler() const;

    void changeState(State);

private:
    void addTask(Task&&);
    void dispatchTask();
    void onSend();
    void onReceive(const Message&);

    void onReceiveAnswer(const Message&);
    void onReceiveRequest(const Message&);

    void onStateChange(SslConnection::State);

private:
    ConnectionPtr mConnection;
    TaskManager mTaskManager;
    Context& mContext;
    Strand mStrand;

    State mState;

    std::optional<DeauthorizationHandler> mDeauthorizationHandler;
    std::optional<NotificationHandler> mNotificationHandler;
    std::optional<StateHandler> mStateHandler;

    friend class Tasker;
};


#endif //ASIOAPPLICATION_CLIENT_HPP
