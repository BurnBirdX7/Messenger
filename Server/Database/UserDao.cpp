#include "UserDao.h"

std::optional<std::vector<UserDao::User>> UserDao::getAll()
{
    std::optional<pqxx::result> res = mPool.query("SELECT * FROM users");

    if(res.has_value()) {
        std::vector<User> vec;
        for (auto row : res.value()) {
            User user;
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

std::optional<UserDao::User> UserDao::getById(int id)
{
    if(id < 1) {
        throw DbException("[UserDao]::getById: id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query("SELECT * FROM users WHERE users.id=" + std::to_string(id));
    if(res.has_value()){
        auto entity = res.value().begin();

        User user;
        user.setId           (entity["id"].get<int>().value());
        user.setPasswordHash (entity["password"].get<std::string>().value());
        user.setName         (entity["name"].get<std::string>().value());
        user.setNickname     (entity["nickname"].get<std::string>().value());

        return user;
    }

    return std::nullopt;
}

bool UserDao::update(User user)
{
    if(mPool.query
    (
        "UPDATE users"
        "  SET password=\'" + user.getPasswordHash() + "\'" +
        ", name=\'"         + user.getName() +"\'" +
        ", nickname=\'"     + user.getNickname() +"\'" +
        "  WHERE users.id=" + std::to_string(user.getId())
    ).has_value()){
        return true;
    }
    return false;
}

bool UserDao::insert(User user)
{
    if(mPool.query
    (
        "INSERT INTO users (id, password, name, nickname) "
        "VALUES(DEFAULT,\'" +
                user.getPasswordHash() + "\',\'" +
                user.getName() + "\',\'" +
                user.getNickname() + "\')"
    ).has_value()){
        return true;
    }
    return false;
}

bool UserDao::deleteById(int id)
{
    if(id < 1) {
        throw DbException("[UserDao]::deleteById: id should be greater than 0");
    }

    if(mPool.query("DELETE FROM users WHERE id=" + std::to_string(id)).has_value()){
        return true;
    }
    return false;
}

std::optional<std::vector<Commons::Data::SessionInfo>> UserDao::getUserSessions(int userId)
{
    if(userId < 1) {
        throw DbException("[UserDao]::getUserSessions: user_id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query
                                            (
                                                "SELECT * FROM sessions "
                                                "WHERE sessions.user_id=" + std::to_string(userId)
                                            );

    if(res.has_value()) {
        std::vector<Commons::Data::SessionInfo> vec;
        for (auto row : res.value()) {
            Commons::Data::SessionInfo session;
            session.setId(row["id"].get<int>().value());
            session.setHash(row["hash"].get<std::string>().value());
            session.setIp4(row["ipv4"].get<int>().value());
            session.setLastActivity(row["last_activity"].get<time_t>().value());
            session.setUserId(row["user_id"].get<int>().value());

            vec.push_back(session);
        }
        return vec;
    }

    return std::nullopt;
}

std::optional<std::vector<Commons::Data::ChatInfo>> UserDao::getUserChats(int userId)
{
    if(userId < 1) {
        throw DbException("[UserDao]::getUserChats: user_id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query
                                            (
                                                "SELECT * FROM chats "
                                                "WHERE chats.id IN (SELECT chat_users.chat_id "
                                                "FROM chat_users "
                                                "WHERE chat_users.user_id=" + std::to_string(userId) + ")"
                                            );
    if(res.has_value()) {
        std::vector<Commons::Data::ChatInfo> vec;
        for (auto row : res.value()) {
            Commons::Data::ChatInfo chat;
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

