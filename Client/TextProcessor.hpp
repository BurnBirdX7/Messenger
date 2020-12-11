#ifndef MESSENGER_TEXTPROCESSOR_HPP
#define MESSENGER_TEXTPROCESSOR_HPP

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>


#include <Data.hpp>
#include <DataBuffers.hpp>

#include "Context.hpp"
#include "Client.hpp"
#include "Tasker.hpp"

#include "CommandSet.hpp"
#include "Container.hpp"

using namespace Commons::Data;


/* TEXT PROCESSOR */

class TextProcessor
{
public:
    using CompletionError = Client::Task::ErrorCode;
    using CompletionHandler = Client::CompletionHandler;

public:
    explicit TextProcessor(Client& client);

    using Task = Commons::Network::Task;
    using ConstBuffer = Commons::Data::ConstBuffer;

public:
    void run();

    void clientNotificationHandler(uint8_t header, ConstBuffer buffer);
    void clientDeauthorizationHandler(const std::string& reason);
    void clientStateHandler(Client::State);

private:
    /* GENERAL ACTIONS */
    void authorize(ConstBuffer);

    void reportProblem(const std::string&);
    void reportProblem(const std::string& msg, CompletionError ec, ConstBuffer reason);
    void reportSuccess(const std::string&);

private:
    Tasker mTasker;
    Container mContainer;

    ChatPtr mActiveChat; // nullptr *or* actual chat
    bool mStopped;

    std::mutex mContainerMutex;
    std::mutex mIOMutex;

    // Isn't in use for now:
    int mSessionId{};
    std::string mSessionHash;

    /* ADDITIONAL */

    enum Error {
        NO_SUCH_COMMAND,
        TOO_FEW_PARAMS,
        TOO_MANY_PARAMS,
        WRONG_PARAMS,
        CANT_GET_THE_CHAT
    };

    void reportProblem(int errorCode);

    void chat_join(const ChatPtr&, const std::string& password);
    void leave_chat(const ChatPtr&);
    void create_chat(const std::string& name);
    void create_chat(const std::string& name, const std::string& password);

    /* COMMAND SETS:  */
public:
    // Login
    void command_login(const std::string& command);      // connected
    // Join chat
    void command_join(const std::string& command);       // authorized

    // Help -a
    void command_help_a(const std::string& command);     // authorized

    // Leave chat
    void command_leave(const std::string& command);      // +

    // Create chat
    void command_create(const std::string& command);     // +

    // get into chat
    void command_msg(const std::string& command);        // +

    // reconnect to server
    void command_reconnect(const std::string& command);  // disconnected

    // disconnect from server
    void command_disconnect(const std::string& command); // connected / authorized

    void command_exit(const std::string& command);

    void command_help_d(const std::string& command);     // disconnected
    void command_help_c(const std::string& command);     // connected

private:
    void setDisconnected();
    void setConnected();
    void setAuthorized();

private:
    CommandSet* mCurrentCommandSet;

    // Set of commands for state "Disconnected"
    CommandSet mDisconnected;

    // Set of commands for state "Connected"
    CommandSet mConnected;

    // Set of commands for state "Authorized"
    CommandSet mAuthorized;
};


#endif //MESSENGER_TEXTPROCESSOR_HPP
