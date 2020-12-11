//
//
//

#ifndef CHATDAO_H
#define CHATDAO_H

#include "AbstractDao.h"
#include "ChatInfo.hpp"

class ChatDao
    : public AbstractDao<Commons::Data::ChatInfo>
{
public:
    using Chat = Commons::Data::ChatInfo;

    std::vector<Chat> getAll();
    Chat getById(int id);
    bool update(Chat chat);
    bool deleteById(int id);
    bool insert(Chat chat);

};


#endif //CHATDAO_H
