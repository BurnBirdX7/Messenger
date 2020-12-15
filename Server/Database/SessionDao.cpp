//
//
//

#include "SessionDao.h"

std::vector<SessionDao::Session> SessionDao::getAll()
{
    std::vector<Session> vec;
    pqxx::result res = mPool.query("SELECT * FROM sessions");

    for (auto row : res) {
        Session session;
        session.setId           (row["id"].get<int>().value());
        session.setHash         (row["hash"].get<std::string>().value());
        session.setIp4          (row["ipv4"].get<int>().value());
        session.setLastActivity (row["last_activity"].get<time_t>().value());
        session.setUserId       (row["user_id"].get<int>().value());

        vec.push_back(session);
    }

    return vec;
}

SessionDao::Session SessionDao::getById(int id)
{
    if(id < 1) {
        throw DbException("[SessionDao]::getById: id should be greater than 0");
    }

    pqxx::result res = mPool.query("SELECT * FROM sessions WHERE id=" + std::to_string(id));
    auto entity = res.begin();

    Session session;
    session.setId           (entity["id"].get<int>().value());
    session.setHash         (entity["hash"].get<std::string>().value());
    session.setIp4          (entity["ipv4"].get<int>().value());
    session.setLastActivity (entity["last_activity"].get<time_t>().value());
    session.setUserId       (entity["user_id"].get<int>().value());

    return session;
}

bool SessionDao::update(SessionDao::Session session)
{
    mPool.query
    (
        "  UPDATE sessions"
        "  SET hash=\'"          + session.getHash() + "\'" +
        ", ipv4="              + std::to_string(session.getIp4()) +
        ", last_activity="     + std::to_string(session.getLastActivity()) + ""
        ", user_id="           + std::to_string(session.getUserId()) +
        "  WHERE sessions.id=" + std::to_string(session.getId())
    );

    return true;
}

bool SessionDao::insert(SessionDao::Session session)
{
    mPool.query
    (
        "INSERT INTO sessions (id, hash, ipv4, last_activity, user_id) "
        "VALUES(DEFAULT,"
                "\'" + session.getHash() + "\'," +
                std::to_string(session.getIp4()) + "," +
                std::to_string(session.getLastActivity()) + "," +
                std::to_string(session.getUserId()) + ")"
    );

    return true;
}

bool SessionDao::deleteById(int id)
{
    if(id < 1) {
        throw DbException("[SessionDao]::getById: id should be greater than 0");
    }

    mPool.query("DELETE FROM sessions WHERE id=" + std::to_string(id));
    return true;
}
