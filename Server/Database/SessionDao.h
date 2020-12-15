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

    std::vector<Session> getAll() override;
    Session getById(int id) override;
    bool    update(Session session) override;
    bool    deleteById(int id) override;
    bool    insert(Session session) override;
};


#endif //SESSIONDAO_H
