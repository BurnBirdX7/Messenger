//
//
//

#ifndef MESSAGEDAO_H
#define MESSAGEDAO_H

#include "TemplateDao.h"
#include "ChatMessage.hpp"
#include "DbException.h"

class MessageDao
        : public TemplateDao<Commons::Data::ChatMessage>
{
public:
    using Message = Commons::Data::ChatMessage;

    std::optional<std::vector<Message>> getAll() override;
    std::optional<Message> getById(int id) override;
    bool update(Message msg) override;
    bool insert(Message msg) override;
    bool deleteById(int id) override;
};


#endif //MESSAGEDAO_H
