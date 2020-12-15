//
//
//

#ifndef MESSAGEDAO_H
#define MESSAGEDAO_H

#include "AbstractDao.h"
#include "ChatMessage.hpp"
#include "DbException.h"

class MessageDao
        : public AbstractDao<Commons::Data::ChatMessage>
{
public:
    using Message = Commons::Data::ChatMessage;

    std::vector<Message> getAll() override;
    Message getById(int id) override;
    bool    update(Message msg) override;
    bool    insert(Message msg) override;
    bool    deleteById(int id) override;
};


#endif //MESSAGEDAO_H
