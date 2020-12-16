//
//
//

#ifndef CHATDAO_H
#define CHATDAO_H

#include "TemplateDao.h"
#include "ChatInfo.hpp"
#include "UserInfo.hpp"
#include "DbException.h"

class ChatDao
        : public TemplateDao<Commons::Data::ChatInfo>
{
public:
    using Chat = Commons::Data::ChatInfo;

    std::optional<std::vector<Chat>> getAll() override;
    std::optional<Chat> getById(int id) override;
    bool update(Chat chat) override;
    bool insert(Chat chat) override;
    bool deleteById(int id) override;

    std::optional<std::vector<int>> getChatUsersSessionsId(int chatId);
    std::optional<std::vector<Commons::Data::UserInfo>> getChatUsers(int chatId);
};


#endif //CHATDAO_H
