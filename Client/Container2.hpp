#ifndef MESSENGER_CONTAINER2_HPP
#define MESSENGER_CONTAINER2_HPP

#include <cassert>
#include <set>
#include <map>
#include <list>
#include <string>
#include <optional>
#include <algorithm>

#include <Data.hpp>


// Message
using Message = Commons::Data::ChatMessage;
using MessagePack = Commons::Data::ChatMessagePack;

bool operator<(const Message& lhs, const Message& rhs);
bool operator<(const Message& lhs, time_t rhs);
bool operator<(time_t lhs, const Message& rhs);
// ! Message


class MyLess {
public:
    using is_transparent = int;

    template <class T, class U>
    constexpr bool operator() (T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) < std::forward<U>(rhs);
    }
};

// User
using User = Commons::Data::UserInfo;
// ! User

// Chat
using ChatInfo = Commons::Data::ChatInfo;
class Chat
{
public:
    using Ptr = const Chat*;
    using MsgSet = std::set<Message, MyLess>;
    using Iter = MsgSet::const_iterator;
    using Range = std::pair<Iter, Iter>;

public:
    explicit Chat() = default;
    explicit Chat(ChatInfo );

    void addMessage(const Message&);
    void addParticipant(int id);

    [[nodiscard]] int getId() const;
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::string& getTitle() const;
    [[nodiscard]] bool hasPassword() const;
    [[nodiscard]] bool isDirect() const;

    [[nodiscard]] const MsgSet& getMessages() const;
    [[nodiscard]] Range getMessages(time_t timeOld, time_t timeNew) const;
    [[nodiscard]] Range getNMessages(size_t n = 20) const;
    [[nodiscard]] Range getNMessages(time_t timeNew, size_t n = 20) const;
    [[nodiscard]] Range getNewMessages(time_t timeOld) const;

    [[nodiscard]] const std::set<int>& getParticipants() const;

private:
    MsgSet mList;
    ChatInfo mInfo;
    std::set<int> mParticipants;

};
// ! Message

class Container2
{
public:
    template <class Type>
    using Iter = typename std::map<int, Type>::const_iterator;

public:
    void addUser(const User&);
    void addChat(const Chat&);
    void addMessage(const Message& msg);
    void addMessagePack(const MessagePack&);

    void removeUser(int id);
    void removeUser(const std::string& nickname);
    void removeChat(int id);
    void removeChat(const std::string& nickname);

    [[nodiscard]] Iter<Chat> findChat(int id) const;
    [[nodiscard]] Iter<Chat> findChat(const std::string& nickname) const;
    [[nodiscard]] Iter<User> findUser(int id) const;
    [[nodiscard]] Iter<User> findUser(const std::string& nickname) const;

    [[nodiscard]] std::optional<Chat> getChat(int id) const;
    [[nodiscard]] std::optional<Chat> getChat(const std::string& nickname) const;
    [[nodiscard]] std::optional<User> getUser(int id) const;
    [[nodiscard]] std::optional<User> getUser(const std::string& nickname) const;

    [[nodiscard]] std::optional<Chat> getDirectChat(int userId_1, int userId_2);

    [[nodiscard]] std::vector<User> getChatUsers(int id) const;
    [[nodiscard]] std::vector<User> getChatUsers(const std::string& nickname) const;
    [[nodiscard]] std::vector<User> getChatUsers(const Chat& chat) const;

private:
    std::map<int, Chat> mChats;
    std::map<int, User> mUsers;

};


#endif //MESSENGER_CONTAINER2_HPP
