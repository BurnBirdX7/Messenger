#ifndef MESSENGER_TEXTPROCESSOR_HPP
#define MESSENGER_TEXTPROCESSOR_HPP

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <queue>

#include <Data.hpp>
#include <DataBuffers.hpp>

#include <Network/Purpose.hpp>

#include "Context.hpp"
#include "Client.hpp"
#include "Tasker.hpp"

#include "CommandSet.hpp"
#include "Container2.hpp"

using namespace Commons::Data;

/* TEXT PROCESSOR */

class TextProcessor
{
public:
    using CompletionError = Client::Task::ErrorCode;
    using CompletionHandler = Client::CompletionHandler;

    using Purpose = Commons::Network::Purpose;

public:
    explicit TextProcessor(Client& client);

    using Task = Commons::Network::Task;
    using ConstBuffer = Commons::Data::ConstBuffer;
    using Queue = std::priority_queue<Message, std::vector<Message>, std::less<>>;

public:
    // Main cycle
    void run();
    void runChat(const Chat&);

    void clientNotificationHandler(uint8_t header, ConstBuffer buffer);
    void clientStateHandler(Client::State);

private: /* GENERAL ACTIONS */
    void reportSuccess(const std::string&);
    void reportProblem(const std::string&);
    void reportProblem(const std::string& msg, CompletionError ec, ConstBuffer reason);

private:
    Tasker mTasker;
    Container2 mContainer;

    Queue mMessageQueue; // Message queue for ActiveChat mode

    bool mStopped;

    std::mutex mContainerMutex;
    std::mutex mIOMutex;

    // Isn't in use for now:
    int mUserId;
    int mSessionId;
    std::string mSessionHash;

private: /* ADDITIONAL */
    enum Error {
        NO_SUCH_COMMAND,
        TOO_FEW_PARAMS,
        TOO_MANY_PARAMS,
        WRONG_PARAMS,
        CANT_GET_THE_CHAT
    };

    void reportProblem(int errorCode);

    void authorize_from_login(ConstBuffer buffer);
    void authorize_from_session(ConstBuffer buffer);

    void _join_chat(const std::string& name, const std::string& password);

    /* COMMAND SETS:  */
public:
    // Login
    void command_login(const std::string& command);      // connected

    // Join chat
    void command_join(const std::string& command);       // authorized

    // Leave chat
    void command_leave(const std::string& command);      // authorized

    // Create chat
    void command_create(const std::string& command);     // authorized

    // Start chat
    void command_start(const std::string& command);      // authorized

    // Get into the group chat
    void command_msg(const std::string& command);        // authorized

    // Get into the private chat
    void command_pmsg(const std::string& command);       // authorized

    // Reconnect to server
    void command_reconnect(const std::string& command);  // disconnected

    // Disconnect from server
    void command_disconnect(const std::string& command); // connected / authorized

    void command_exit();

    void command_help_a(const std::string& command);     // authorized
    void command_help_c(const std::string& command);     // connected
    void command_help_d(const std::string& command);     // disconnected

private:
    void _initDisconnected();
    void _initConnected();
    void _initAuthorized();

private:
    CommandSet* mCurrentCommandSet;

    // Set of commands for state "Disconnected"
    CommandSet mDisconnected_CommandSet;

    // Set of commands for state "Connected"
    CommandSet mConnected_CommandSet;

    // Set of commands for state "Authorized"
    CommandSet mAuthorized_CommandSet;
};


#endif //MESSENGER_TEXTPROCESSOR_HPP
