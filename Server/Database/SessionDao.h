//
//
//

#ifndef SESSIONDAO_H
#define SESSIONDAO_H

#include "AbstractDao.h"
#include "SessionInfo.hpp"
#include "DbException.h"

class SessionDao
        : public AbstractDao<Commons::Data::SessionInfo>
{
public:
    using Session = Commons::Data::SessionInfo;

    std::optional<std::vector<Session>> getAll() override;
    std::optional<Session> getById(int id) override;
    bool update(Session session) override;
    bool deleteById(int id) override;
    bool insert(Session session) override;

    std::optional<int> createSession(int ip4);
};


#endif //SESSIONDAO_H
