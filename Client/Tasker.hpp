#ifndef MESSENGER_TASKER_HPP
#define MESSENGER_TASKER_HPP

#include <string>

#include <Network/Task.hpp>
#include <Data/Types.hpp>
#include <Data/Buffer.hpp>
#include <Data/BufferComposer.hpp>
#include <Data/BufferDecomposer.hpp>

class Client;
class Context;

class Tasker
{
public:
    using Task = Commons::Network::Task;
    using CompletionHandler = Task::CompletionHandler;
    using ConstBuffer = Commons::Data::ConstBuffer;

public:
    explicit Tasker(Client&);

    Client& getClient();
    Context& getContext();

    // REQUESTS
    // ~ Basic Authentication ~
    void login(const std::string& login, const std::string& password, const CompletionHandler& handler);
    void logoff(const CompletionHandler& handler);
    void restoreSession(int sessionId, const std::string& hash, const CompletionHandler& handler);
    void registerUser(const std::string& login, const std::string& password, const CompletionHandler& handler);

    // ~ Basic Info ~
    void getUserId(const std::string& nickname, const CompletionHandler& handler);
    void getChatId(const std::string& chatName, const CompletionHandler& handler);
    void getChatById(int chatId, const CompletionHandler& handler);
    void getChatByName(const std::string& chatName, const CompletionHandler& handler);
    void getDirectChatById(int userId, const CompletionHandler& handler);
    void getDirectChatByName(const std::string& name, const CompletionHandler& handler);

    // ~ Work with chat ~
    void joinChat(int chatId, const CompletionHandler& handler);
    void joinChat(int chatId, const std::string& password, const CompletionHandler& handler);
    void leaveChat(int chatId, const CompletionHandler& handler);
    void createChat(const std::string& chatName, const CompletionHandler& handler);
    void createChat(const std::string& chatName, const std::string& password, const CompletionHandler& handler);
    void startChat(int userId, const CompletionHandler& handler);
    void deleteChat(int chatId, const CompletionHandler& handler);
    void renameChat(int chatId, const std::string& chatName, const CompletionHandler& handler);
    void changeChatTitle(int chatId, const std::string& chatName, const CompletionHandler& handler);
    void setChatAdmin(int chatId, int userId, const CompletionHandler& handler);
    void removeChatAdmin(int chatId, int userId, const CompletionHandler& handler);
    void getUserChats(const CompletionHandler& handler);

    // ~ Messaging ~
    void sendChatMessage(int chatId, const std::string& content, const CompletionHandler& handler);
    void requestChatMessage(int chatId, time_t timeOlder, const CompletionHandler& handler);
    void requestChatMessage(int chatId, time_t timeOlder, time_t timeNewer, const CompletionHandler& handler);
    void markAsSeen(int chatId, time_t whenSeen, const CompletionHandler& handler);
    void getWhenSeen(int chatId,  const CompletionHandler& handler);
    void removeMessage(int messageId, const CompletionHandler& handler);
    void deleteMessage(int messageId, const CompletionHandler& handler);

    // ~ Advanced Info ~
    void getUserData(int userId, const CompletionHandler& handler);
    void getUserData(const std::string& name, const CompletionHandler& handler);
    void getUserPrivateData(const CompletionHandler& handler);
    void getListOfSessions(const CompletionHandler& handler);

private:
    Client& mClient;
    Context& mContext;

};


#endif //MESSENGER_TASKER_HPP
