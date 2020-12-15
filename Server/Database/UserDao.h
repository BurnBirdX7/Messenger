//
//
//

#ifndef USERDAO_H
#define USERDAO_H

#include "AbstractDao.h"
#include "UserInfo.hpp"
#include "DbException.h"

class UserDao
        : public AbstractDao<Commons::Data::UserInfo>
{
public:
    using User = Commons::Data::UserInfo;

    std::vector<User> getAll() override;
    User getById(int id) override;
    bool update(User data) override;
    bool insert(User data) override;
    bool deleteById(int id) override;
};


#endif //USERDAO_H
