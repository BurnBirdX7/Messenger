#include "Tasker.hpp"

// Include incomplete types
#include "Client.hpp"
#include "Context.hpp"

using Commons::Network::Purpose;
using Commons::Data::Buffer;
using Commons::Data::BufferComposer;

Tasker::Tasker(Client& client)
    : mClient(client)
    , mContext(client.getContext())
{}

Client& Tasker::getClient()
{
    return mClient;
}


Context& Tasker::getContext()
{
    return mContext;
}


void Tasker::login(const std::string& login, const std::string& password, const CompletionHandler& handler)
{
    mClient.authorize(login, password, handler);
}

void Tasker::logoff(const CompletionHandler& handler)
{
    mClient.deauthorize(handler);
}

void Tasker::restoreSession(int sessionId, const std::string& hash, const CompletionHandler& handler)
{
    mClient.authorize(sessionId, hash, handler);
}

void Tasker::registerUser(const std::string& login, const std::string& password, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(login)
        .append(password);

    mClient.addTask( Task(Purpose::REGISTER_USER, composer.getVector(), handler) );
}

void Tasker::getUserId(const std::string& nickname, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_USER_ID, Buffer::string(nickname), handler) );
}

void Tasker::getChatId(const std::string& chatName, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_CHAT_ID, Buffer::string(chatName), handler) );
}

void Tasker::getChatById(int chatId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_CHAT_BY_ID, Buffer::primitiveType(chatId), handler) );
}

void Tasker::getChatByName(const std::string& chatName, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_CHAT_BY_NAME, Buffer::string(chatName), handler) );
}

void Tasker::getDirectChatById(int userId, const CompletionHandler& handler)
{
    mClient.addTask(Task(Purpose::GET_DCHAT_BY_ID, Buffer::primitiveType(userId), handler));
}

void Tasker::getDirectChatByName(const std::string& name, const CompletionHandler& handler)
{
    mClient.addTask(Task(Purpose::GET_DCHAT_BY_NAME, Buffer::string(name), handler));
}

void Tasker::joinChat(int chatId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::JOIN_CHAT, Buffer::primitiveType(chatId), handler) );
}

void Tasker::joinChat(int chatId, const std::string& password, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(password);
    mClient.addTask( Task(Purpose::JOIN_CHAT, composer.getVector(), handler) );
}

void Tasker::leaveChat(int chatId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::LEAVE_CHAT, Buffer::primitiveType(chatId), handler) );
}

void Tasker::createChat(const std::string& chatName, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::CREATE_CHAT, Buffer::string(chatName), handler) );
}

void Tasker::createChat(const std::string& chatName, const std::string& password, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatName)
        .append(password);
    mClient.addTask( Task(Purpose::CREATE_CHAT, composer.getVector(), handler) );
}

void Tasker::startChat(int userId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::START_CHAT, Buffer::primitiveType(userId), handler) );
}

void Tasker::deleteChat(int chatId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::DELETE_CHAT, Buffer::primitiveType(chatId), handler) );
}

void Tasker::renameChat(int chatId, const std::string& chatName, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(chatName);

    mClient.addTask( Task(Purpose::RENAME_CHAT, composer.getVector(), handler) );
}

void Tasker::changeChatTitle(int chatId, const std::string& chatName, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(chatName);
    mClient.addTask( Task(Purpose::CH_CHAT_TITLE, composer.getVector(), handler) );
}

void Tasker::setChatAdmin(int chatId, int userId, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(userId);
    mClient.addTask( Task(Purpose::SET_CHAT_ADMIN, composer.getVector(), handler) );
}

void Tasker::removeChatAdmin(int chatId, int userId, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(userId);
    mClient.addTask( Task(Purpose::REM_CHAT_ADMIN, composer.getVector(), handler) );
}

void Tasker::getUserChats(const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_USR_CHATS, handler) );
}

void Tasker::sendChatMessage(int chatId, const std::string& content, const CompletionHandler& handler)
{
    std::vector<ConstBuffer> seq;
    Commons::Data::BufferComposer composer(seq);
    composer
            .append(chatId)
            .append(content);

    Task msgTask(Purpose::SEND_CHAT_MSG, seq, handler);

    mClient.addTask(std::move(msgTask));
}

void Tasker::requestChatMessage(int chatId, time_t timeOlder, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(timeOlder);
    mClient.addTask( Task(Purpose::REQUEST_CHAT_MSG, composer.getVector(), handler) );
}

void Tasker::requestChatMessage(int chatId, time_t timeOlder, time_t timeNewer, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(timeOlder)
        .append(timeNewer);
    mClient.addTask( Task(Purpose::REQUEST_CHAT_MSG, composer.getVector(), handler) );
}

void Tasker::markAsSeen(int chatId, time_t whenSeen, const CompletionHandler& handler)
{
    BufferComposer composer;
    composer
        .append(chatId)
        .append(whenSeen);
    mClient.addTask( Task(Purpose::MARK_SEEN, composer.getVector(), handler) );
}

void Tasker::getWhenSeen(int chatId,  const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_SEEN, Buffer::primitiveType(chatId), handler) );
}

void Tasker::removeMessage(int messageId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::REMOVE_MSG, Buffer::primitiveType(messageId), handler) );
}

void Tasker::deleteMessage(int messageId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::DELETE_MSG, Buffer::primitiveType(messageId), handler) );
}

void Tasker::getUserData(int userId, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_USR_DATA, Buffer::primitiveType(userId), handler) );
}

void Tasker::getUserData(const std::string& name, const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_USR_DATA_NAME, Buffer::string(name), handler) );
}

void Tasker::getUserPrivateData(const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_USR_PR_DATA, handler) );
}

void Tasker::getListOfSessions(const CompletionHandler& handler)
{
    mClient.addTask( Task(Purpose::GET_SESSIONS, handler) );
}


