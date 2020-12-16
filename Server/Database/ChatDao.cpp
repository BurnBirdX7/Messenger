#include "ChatDao.h"

std::optional<std::vector<ChatDao::Chat>> ChatDao::getAll()
{
    std::optional<pqxx::result> res = mPool.query("SELECT * FROM chats");

    if(res.has_value()) {
        std::vector<Chat> vec;
        for (auto row : res.value()) {
            Chat chat;
            chat.setId(row["id"].get<int>().value());
            chat.setTitle(row["title"].get<std::string>().value());
            chat.setIsDirect(row["is_direct"].get<bool>().value());
            chat.setTimeUpdated(row["time_updated"].get<time_t>().value());
            chat.setNickname(row["nickname"].get<std::string>().value());
            chat.setPasswordHash(row["password"].get<std::string>().value());

            vec.push_back(chat);
        }
        return vec;
    }
    return std::nullopt;
}

std::optional<ChatDao::Chat> ChatDao::getById(int id)
{
    if(id < 1) {
        throw DbException("[ChatDao]::getById: id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query("SELECT * FROM chats WHERE id=" + std::to_string(id));

    if(res.has_value()) {
        auto entity = res.value().begin();

        Chat chat;
        chat.setId(entity["id"].get<int>().value());
        chat.setTitle(entity["title"].get<std::string>().value());
        chat.setIsDirect(entity["is_direct"].get<bool>().value());
        chat.setTimeUpdated(entity["time_updated"].get<time_t>().value());
        chat.setNickname(entity["nickname"].get<std::string>().value());
        chat.setPasswordHash(entity["password"].get<std::string>().value());

        return chat;
    }
    return std::nullopt;
}

bool ChatDao::update(Chat chat)
{
    if(mPool.query
    (
        "  UPDATE chats"
        "  SET title=\'"     + chat.getTitle() + "\'," +
        ", is_direct="       + std::to_string(chat.isDirect()) +
        ", time_updated="    + std::to_string(chat.getTimeUpdated()) +
        ", nickname=\'"      + chat.getNickname() + "\'," +
        ", password=\'"      + chat.getPasswordHash() + "\'" +
        "  WHERE chats.id="  + std::to_string(chat.getId())
    ).has_value()){
        return true;
    }
    return false;
}

bool ChatDao::insert(Chat chat)
{
    if(mPool.query
    (
        "INSERT INTO chats (id, title, is_direct, time_updated, nickname, password) "
        "VALUES(DEFAULT,"
                "\'" + chat.getTitle() + "\'," +
                std::to_string(chat.isDirect()) + "," +
                std::to_string(chat.getTimeUpdated()) + ","
                "\'" + chat.getNickname() + "\',"
                "\'" + chat.getPasswordHash() + "\')"
    ).has_value()){
        return true;
    }
    return false;
}

bool ChatDao::deleteById(int id)
{
    if(id < 1) {
        throw DbException("[ChatDao]::deleteById: id should be greater than 0");
    }

    if(mPool.query("DELETE FROM chats WHERE id=" + std::to_string(id)).has_value()){
        return true;
    }
    return false;
}

std::optional<std::vector<int>> ChatDao::getChatUsersSessionsId(int chatId)
{
    if(chatId < 1) {
        throw DbException("[ChatDao]::getChatUsersSessionsId: chat_id should be greater than 0");
    }

    std::vector<int> vec;
    std::optional<pqxx::result> res = mPool.query
                                            (
                                                "SELECT sessions.id FROM sessions "
                                                "JOIN chat_users "
                                                "ON sessions.user_id=chat_users.user_id AND chat_users.chat_id=" +
                                                std::to_string(chatId)
                                            );

    if(res.has_value()) {
        for (auto row : res.value()) {
            vec.push_back(row["id"].get<int>().value());
        }
        return vec;
    }
    return std::nullopt;
}

std::optional<std::vector<Commons::Data::UserInfo>> ChatDao::getChatUsers(int chatId)
{
    if(chatId < 1) {
        throw DbException("[ChatDao]::getChatUsers: id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query
                                            (
                                                "SELECT * FROM users "
                                                "WHERE users.id IN "
                                                "(SELECT chat_users.user_id "
                                                "FROM chat_users "
                                                "WHERE chat_users.chat_id=" + std::to_string(chatId) + ")"
                                            );
    if(res.has_value()) {
        std::vector<Commons::Data::UserInfo> vec;
        for (auto row : res.value()) {
            Commons::Data::UserInfo user;
            user.setId           (row["id"].get<int>().value());
            user.setPasswordHash (row["password"].get<std::string>().value());
            user.setName         (row["name"].get<std::string>().value());
            user.setNickname     (row["nickname"].get<std::string>().value());

            vec.push_back(user);
        }
        return vec;
    }
    return std::nullopt;
}
