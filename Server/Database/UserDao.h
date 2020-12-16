//
//
//

#ifndef USERDAO_H
#define USERDAO_H

#include "TemplateDao.h"
#include "UserInfo.hpp"
#include "ChatInfo.hpp"
#include "SessionInfo.hpp"
#include "DbException.h"

class UserDao
        : public TemplateDao<Commons::Data::UserInfo>
{
public:
    using User = Commons::Data::UserInfo;

    std::optional<std::vector<User>> getAll() override;
    std::optional<User> getById(int id) override;
    bool update(User data) override;
    bool insert(User data) override;
    bool deleteById(int id) override;

    std::optional<std::vector<Commons::Data::SessionInfo>> getUserSessions(int userId);
    std::optional<std::vector<Commons::Data::ChatInfo>> getUserChats(int userId);
};


#endif //USERDAO_H
