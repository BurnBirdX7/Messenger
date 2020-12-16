//
//
//

#include "SessionDao.h"

std::optional<std::vector<SessionDao::Session>> SessionDao::getAll()
{
    std::optional<pqxx::result> res = mPool.query("SELECT * FROM sessions");

    if(res.has_value()) {
        std::vector<Session> vec;
        for (auto row : res.value()) {
            Session session;
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

std::optional<SessionDao::Session> SessionDao::getById(int id)
{
    if(id < 1) {
        throw DbException("[SessionDao]::getById: id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query("SELECT * FROM sessions WHERE id=" + std::to_string(id));
    if(res.has_value()) {
        auto entity = res.value().begin();

        Session session;
        session.setId(entity["id"].get<int>().value());
        session.setHash(entity["hash"].get<std::string>().value());
        session.setIp4(entity["ipv4"].get<int>().value());
        session.setLastActivity(entity["last_activity"].get<time_t>().value());
        session.setUserId(entity["user_id"].get<int>().value());

        return session;
    }
    return std::nullopt;
}

bool SessionDao::update(SessionDao::Session session)
{
    if(mPool.query
    (
        "  UPDATE sessions"
        "  SET hash=\'"          + session.getHash() + "\'" +
        ", ipv4="              + std::to_string(session.getIp4()) +
        ", last_activity="     + std::to_string(session.getLastActivity()) + ""
        ", user_id="           + std::to_string(session.getUserId()) +
        "  WHERE sessions.id=" + std::to_string(session.getId())
    ).has_value()){
        return true;
    }
    return false;
}

bool SessionDao::insert(SessionDao::Session session)
{
    if(mPool.query
    (
        "INSERT INTO sessions (id, hash, ipv4, last_activity, user_id) "
        "VALUES(DEFAULT,"
                "\'" + session.getHash() + "\'," +
                std::to_string(session.getIp4()) + "," +
                std::to_string(session.getLastActivity()) + "," +
                std::to_string(session.getUserId()) + ")"
    ).has_value()){
        return true;
    }
    return false;
}

bool SessionDao::deleteById(int id)
{
    if(id < 1) {
        throw DbException("[SessionDao]::getById: id should be greater than 0");
    }

    if(mPool.query("DELETE FROM sessions WHERE id=" + std::to_string(id)).has_value()){
        return true;
    }
    return false;
}

std::optional<int> SessionDao::createSession(int ip4) {
    std::optional<pqxx::result> res = mPool.query
                                            (
                                                "INSERT INTO sessions "
                                                "VALUES (DEFAULT,NULL," + std::to_string(ip4) + ",NULL,NULL) "
                                                "RETURNING id"
                                            );
    if(res.has_value()) {
        auto entity = res.value().begin();
        return entity["id"].get<int>().value();
    }
    return std::nullopt;
}
