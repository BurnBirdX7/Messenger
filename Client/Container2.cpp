#include "Container2.hpp"

#include <utility>

// MESSAGE //

bool operator<(const Message& lhs, const Message& rhs)
{
    return lhs.getTimeCreated() < rhs.getTimeCreated();
}

bool operator<(const Message& lhs, time_t rhs)
{
    return lhs.getTimeCreated() < rhs;
}

bool operator<(time_t lhs, const Message& rhs)
{
    return lhs < rhs.getTimeCreated();
}

// CHAT //

Chat::Chat(ChatInfo info)
    : mInfo(std::move(info))
{}

void Chat::addMessage(const Message& msg)
{
    mList.insert(msg);
}

void Chat::addParticipant(int id)
{
    mParticipants.insert(id);
}

int Chat::getId() const
{
    return mInfo.getId();
}

const std::string& Chat::getName() const
{
    return mInfo.getNickname();
}

const std::string& Chat::getTitle() const
{
    return mInfo.getTitle();
}

bool Chat::hasPassword() const
{
    return mInfo.hasPassword();
}

bool Chat::isDirect() const
{
    return mInfo.isDirect();
}

const Chat::MsgSet& Chat::getMessages() const
{
    return mList;
}

Chat::Range Chat::getMessages(time_t timeOld, time_t timeNew) const
{
    auto lower = mList.lower_bound(timeOld);
    auto upper = mList.upper_bound(timeNew);

    return {lower, upper};
}

Chat::Range Chat::getNMessages(size_t n) const
{
    auto upper = mList.end();

    auto lower = upper;
    for (size_t i = 0; i < n; ++i)
        --lower;

    return {lower, upper};
}

Chat::Range Chat::getNMessages(time_t timeNew, size_t n) const
{
    auto upper = mList.upper_bound(timeNew);

    auto lower = upper;
    for (size_t i = 0; i < n; ++i)
        --lower;

    return {lower, upper};
}

Chat::Range Chat::getNewMessages(time_t timeOld) const
{
    auto lower = mList.lower_bound(timeOld);

    return {lower, mList.end()};
}

const std::set<int>& Chat::getParticipants() const
{
    return mParticipants;
}

// CONTAINER //

void Container2::addUser(const User& user)
{
    mUsers[user.getId()] = user;
}

void Container2::addChat(const Chat& chat)
{
    mChats[chat.getId()] = chat;
}

void Container2::addMessage(const Message& msg)
{
    auto it = mChats.find(msg.getChatId());
    if (it == mChats.end())
        return; // If there's no such chat - just ignore

    it->second.addMessage(msg);
}

void Container2::addMessagePack(const MessagePack& pack)
{
    for (const auto& msg : pack.getVector())
        addMessage(msg);
}

void Container2::removeUser(int id)
{
    mUsers.erase(id);
}

void Container2::removeUser(const std::string& nickname)
{
    auto it = findUser(nickname);
    mUsers.erase(it);
}

void Container2::removeChat(int id)
{
    mUsers.erase(id);
}

void Container2::removeChat(const std::string& nickname)
{
    auto it = findChat(nickname);
    mChats.erase(it);
}

Container2::Iter<Chat> Container2::findChat(int id) const
{
    return mChats.find(id);
}

Container2::Iter<Chat> Container2::findChat(const std::string& nickname) const
{
    auto predicate = [&] (const std::pair<int, Chat>& pair) {
        return pair.second.getName() == nickname;
    };

    return std::find_if(mChats.begin(), mChats.end(), predicate);
}

Container2::Iter<User> Container2::findUser(int id) const
{
    return mUsers.find(id);
}

Container2::Iter<User> Container2::findUser(const std::string& nickname) const
{
    auto predicate = [&] (const std::pair<int, User>& pair) {
        return pair.second.getName() == nickname;
    };

    return std::find_if(mUsers.begin(), mUsers.end(), predicate);
}

std::optional<Chat> Container2::getChat(int id) const
{
    auto it = findChat(id);

    if (it == mChats.end())
        return std::nullopt;

    return it->second;
}

std::optional<Chat> Container2::getChat(const std::string& nickname) const
{
    auto it = findChat(nickname);

    if (it == mChats.end())
        return std::nullopt;

    return it->second;
}

std::optional<User> Container2::getUser(int id) const
{
    auto it = findUser(id);

    if (it == mUsers.end())
        return std::nullopt;

    return it->second;
}

std::optional<User> Container2::getUser(const std::string& nickname) const
{
    auto it = findUser(nickname);

    if (it == mUsers.end())
        return std::nullopt;

    return it->second;
}

std::optional<Chat> Container2::getDirectChat(int userId_1, int userId_2)
{
    auto predicate = [=] (const std::pair<int, Chat>& pair) {
        const auto& chat = pair.second;

        if (!chat.isDirect())
            return false;

        const auto& p = chat.getParticipants();

        assert (p.size() == 2);

        auto it1 = p.find(userId_1);
        auto it2 = p.find(userId_2);

        return (it1 != p.end()) && (it2 != p.end());
    };

    auto it = std::find_if(mChats.begin(), mChats.end(), predicate);
    if (it == mChats.end())
        return std::nullopt;

    return it->second;
}

std::vector<User> Container2::getChatUsers(int id) const
{
    auto chat = findChat(id);
    if (chat == mChats.end())
        return {};

    const auto& userIds = chat->second.getParticipants();

    std::vector<User> vec;
    vec.reserve(userIds.size());

    for (const auto& uId : userIds) {
        auto opt = getUser(uId);
        assert(opt.has_value());
        vec.push_back(opt.value_or(User()));
    }

    return vec;
}

// Ugly copypasted
std::vector<User> Container2::getChatUsers(const std::string& nickname) const
{
    auto chat = findChat(nickname);
    if (chat == mChats.end())
        return {};

    const auto& userIds = chat->second.getParticipants();

    std::vector<User> vec;
    vec.reserve(userIds.size());

    for (const auto& uId : userIds) {
        auto opt = getUser(uId);
        assert(opt.has_value());
        vec.push_back(opt.value_or(User()));
    }

    return vec;
}

// Ugly copypasted
std::vector<User> Container2::getChatUsers(const Chat& chat) const
{
    const auto& userIds = chat.getParticipants();
    std::vector<User> vec;
    vec.reserve(userIds.size());

    for (const auto& uId : userIds) {
        auto opt = getUser(uId);
        assert(opt.has_value());
        vec.push_back(opt.value_or(User()));
    }

    return vec;
}
