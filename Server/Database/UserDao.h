//
//
//

#ifndef USERDAO_H
#define USERDAO_H

#include "AbstractDao.h"
#include "UserInfo.hpp"

class UserDao
        : public AbstractDao<Commons::Data::UserInfo>
{
public:
    using User = Commons::Data::UserInfo;

    std::vector<User> getAll();
    User getById(int id);
    bool update(User data);
    bool deleteById(int id);
    bool insert(User data);

};


#endif //USERDAO_H
