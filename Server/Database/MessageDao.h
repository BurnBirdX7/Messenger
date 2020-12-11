//
//
//

#ifndef MESSAGEDAO_H
#define MESSAGEDAO_H

#include "AbstractDao.h"
#include "ChatMessage.hpp"

class MessageDao : public AbstractDao<Commons::Data::ChatMessage>
{
public:
    using Message = Commons::Data::ChatMessage;

    std::vector<Message> getAll();
    Message getById(int id);
    bool update(Message msg);
    bool deleteById(int id);
    bool insert(Message msg);

};


#endif //MESSAGEDAO_H
