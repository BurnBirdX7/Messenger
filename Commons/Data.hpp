#ifndef MESSENGER_DATA_HPP
#define MESSENGER_DATA_HPP

#include "Data/DataPack.hpp"
#include "Data/ChatInfo.hpp"
#include "Data/ChatMessage.hpp"
#include "Data/UserInfo.hpp"
#include "Data/SessionInfo.hpp"

namespace Commons::Data {

    using ChatInfoPack = DataPack<ChatInfo>;
    using ChatMessagePack = DataPack<ChatMessage>;
    using UserInfoPack = DataPack<UserInfo>;
    using SessionList = DataPack<SessionInfo>;

}

#endif //MESSENGER_DATA_HPP
