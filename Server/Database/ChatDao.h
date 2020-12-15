//
//
//

#ifndef CHATDAO_H
#define CHATDAO_H

#include "AbstractDao.h"
#include "ChatInfo.hpp"
#include "DbException.h"

class ChatDao
        : public AbstractDao<Commons::Data::ChatInfo>
{
public:
    using Chat = Commons::Data::ChatInfo;

    std::vector<Chat> getAll() override;
    Chat getById(int id) override;
    bool update(Chat chat) override;
    bool insert(Chat chat) override;
    bool deleteById(int id) override;

};


#endif //CHATDAO_H
