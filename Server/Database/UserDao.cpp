#include "UserDao.h"

std::vector<UserDao::User> UserDao::getAll()
{
    std::vector<User> vec;
    pqxx::result res = mPool.query("SELECT * FROM users;");

    for (auto row : res) {
        User user;
        user.setId           (row["id"].get<int>().value());
        user.setPasswordHash (row["password"].get<std::string>().value());
        user.setName         (row["name"].get<std::string>().value());
        user.setNickname     (row["nickname"].get<std::string>().value());

        vec.push_back(user);
    }

    return vec;
}

UserDao::User UserDao::getById(int id)
{
    pqxx::result res = mPool.query("SELECT * FROM users WHERE users.id=" + std::to_string(id));
    auto entity = res.begin();

    User user;
    user.setId           (entity["id"].get<int>().value());
    user.setPasswordHash (entity["password"].get<std::string>().value());
    user.setName         (entity["name"].get<std::string>().value());
    user.setNickname     (entity["nickname"].get<std::string>().value());

    return user;
}

bool UserDao::update(User user)
{
    mPool.query("UPDATE users"
                "  SET password=\'" + user.getPasswordHash() + "\'" +
                ", name=\'" + user.getName() +"\'" +
                ", nickname=\'" + user.getNickname() +"\'" +
                "  WHERE users.id=" + std::to_string(user.getId())
                );
    return true;
}

bool UserDao::deleteById(int id)
{
    mPool.query("DELETE FROM users WHERE id=" + std::to_string(id));
    return true;
}

bool UserDao::insert(User user)
{
    mPool.query("INSERT INTO users (id, password, name, nickname) "
                "VALUES(DEFAULT,\'" + user.getPasswordHash() + "\',\'" + user.getName() + "\',\'" + user.getNickname() + "\')");
    return true;
}
