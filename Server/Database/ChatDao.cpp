#include "ChatDao.h"

std::vector<ChatDao::Chat> ChatDao::getAll()
{
    std::vector<Chat> vec;
    pqxx::result res = mPool.query("SELECT * FROM chats;");

    for (auto row : res) {
        Chat chat;
        chat.setId           (row["id"].get<int>().value());
        chat.setTitle        (row["title"].get<std::string>().value());
        chat.setIsDirect     (row["is_direct"].get<bool>().value());
        chat.setTimeUpdated  (row["time_updated"].get<time_t>().value());
        chat.setNickname     (row["nickname"].get<std::string>().value());
        chat.setPasswordHash (row["password"].get<std::string>().value());

        vec.push_back(chat);
    }

    return vec;
}

ChatDao::Chat ChatDao::getById(int id)
{
    pqxx::result res = mPool.query("SELECT * FROM chats WHERE id=" + std::to_string(id));
    auto entity = res.begin();

    Chat chat;
    chat.setId           (entity["id"].get<int>().value());
    chat.setTitle        (entity["title"].get<std::string>().value());
    chat.setIsDirect     (entity["is_direct"].get<bool>().value());
    chat.setTimeUpdated  (entity["time_updated"].get<time_t>().value());
    chat.setNickname     (entity["nickname"].get<std::string>().value());
    chat.setPasswordHash (entity["password"].get<std::string>().value());

    return chat;
}

bool ChatDao::update(Chat chat)
{
    mPool.query("  UPDATE chats"
                "  SET title=" + chat.getTitle() +
                ", is_direct=" + std::to_string(chat.isDirect()) +
                ", time_updated=" + std::to_string(chat.getTimeUpdated()) +
                ", nickname=" + chat.getNickname() +
                ", password=" + chat.getPasswordHash() +
                "  WHERE chats.id=" + std::to_string(chat.getId())
                );
    return true;
}

bool ChatDao::deleteById(int id)
{
    mPool.query("DELETE FROM chats WHERE id=" + std::to_string(id));
    return true;
}

bool ChatDao::insert(Chat chat)
{
    mPool.query("INSERT INTO chats (id, title, is_direct, time_updated, nickname, password) "
                "VALUES(DEFAULT,"
                "\'" + chat.getTitle() + "\'," +
                std::to_string(chat.isDirect()) + "," +
                std::to_string(chat.getTimeUpdated()) + ","
                "\'" + chat.getNickname() + "\',"
                "\'" + chat.getPasswordHash() + "\')"
                );

    return true;
}
