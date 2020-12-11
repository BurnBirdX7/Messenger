#ifndef MESSENGER_CONTAINER_HPP
#define MESSENGER_CONTAINER_HPP

#include <memory>
#include <functional>
#include <variant>
#include <utility>

#include <Data.hpp>
#include <DataBuffers.hpp>

#include "Tasker.hpp"

using namespace Commons::Data;

/* ADDITIONAL DATA TYPES */

struct User {
    int id;
    std::string name;
    std::string nickname;
};

using UserPtr = std::shared_ptr<User>;

struct Message {
    int id;
    std::string text;
    UserPtr author;
};

using MessagePtr = std::shared_ptr<Message>;

struct Chat {
    int id;
    std::string title;
    std::string nickname;
    bool hasPassword;
    std::list<MessagePtr> messages;
};

using ChatPtr = std::shared_ptr<Chat>;

class Container
{
public:
    using loadedHandler = const std::function<void ()>&;
    using userLoadedHandler = const std::function<void (bool, UserPtr)>&;
    using chatLoadedHandler = const std::function<void (bool, ChatPtr)>&;

    using CompletionError = Tasker::Task::ErrorCode;

    using id_t = std::variant<int, std::string>;

public:
    explicit Container(Tasker& tasker);

    void setCurrentUser(const id_t& id_or_name);
    UserPtr getCurrentUser() const;

    void loadChat(const id_t& id_or_name, chatLoadedHandler handler);
    void loadUser(const id_t& id_or_nickname, userLoadedHandler handler);

    void loadUserChats();
    void loadUserMessages();

    void loadMessages(int chatId, time_t timeOld, loadedHandler);

    ChatPtr getChat(const id_t&) const;
    UserPtr getUser(const id_t&) const;

    void addUserChat(const ChatPtr&);
    void removeUserChat(const ChatPtr&);

private:
    UserPtr mCurrentUser;

    std::vector<ChatPtr> mUserChats;
    std::vector<UserPtr> mLoadedUsers;
    std::vector<ChatPtr> mLoadedChats;

    Tasker mTasker;
};


#endif //MESSENGER_CONTAINER_HPP
