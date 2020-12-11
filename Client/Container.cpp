#include "Container.hpp"


Container::Container(Tasker& tasker)
    : mTasker(tasker)
{}

void Container::setCurrentUser(const id_t& id_or_name)
{
    UserPtr usr = getUser(id_or_name);
    if (!usr) {
        loadUser(id_or_name, [this] (bool ok, UserPtr ptr) {
            if (ok)
                mCurrentUser = std::move(ptr);
        });
    }
    else {
        mCurrentUser = usr;
    }
}

UserPtr Container::getCurrentUser() const
{
    return mCurrentUser;
}

void Container::loadChat(const id_t& id_or_name, chatLoadedHandler handler)
{
    auto load_handler = [this, handler](auto ec, auto buffer) {

        if (ec == CompletionError::OK) {
            ChatInfo info;
            info.fillFromBuffer(buffer);
            ChatPtr ptr = std::make_shared<Chat>();

            ptr->id = info.getId();
            ptr->hasPassword = info.hasPassword();
            ptr->nickname = info.getNickname();
            ptr->title = info.getTitle();

            if (handler)
                handler(true, ptr);
        }
        else if (handler) {
            handler(false, nullptr);
        }

    };

    if (std::holds_alternative<int>(id_or_name))
        mTasker.getChatById(std::get<int>(id_or_name), load_handler);
    else if (std::holds_alternative<std::string>(id_or_name))
        mTasker.getChatByName(std::get<std::string>(id_or_name), load_handler);
    else if (handler)
        handler(false, nullptr);

}

void Container::loadUser(const id_t& id_or_nickname, userLoadedHandler handler)
{
    auto load_handler = [this, handler] (auto ec, auto buffer) {

        if (ec == CompletionError::OK) {
            UserInfo info;
            info.fillFromBuffer(buffer);
            UserPtr ptr = std::make_shared<User>();

            ptr->nickname = info.getNickname();
            ptr->id = info.getId();
            ptr->name = info.getName();

            if (handler)
                handler(true, ptr);
        }
        else if (handler)
            handler(false, nullptr);

    };

    if (std::holds_alternative<int>(id_or_nickname)) {
        mTasker.getUserData(std::get<int>(id_or_nickname), load_handler);
    }
    else if (std::holds_alternative<std::string>(id_or_nickname)) {
        mTasker.getUserId(
                std::get<std::string>(id_or_nickname),
                [this, handler, load_handler] (auto ec, auto buffer)
                {
                    if (ec == CompletionError::OK) {
                        int id = BufferDecomposer(buffer).get<int>();
                        mTasker.getUserData(id, load_handler); // handler will be invoked it there
                    }
                    else if (handler)
                        handler(false, nullptr);

                }
        );
    }
    else if (handler)
        handler(false, nullptr);


}

void Container::loadUserChats()
{
    mTasker.getUserChats([this] (auto ec, auto buffer) {

        if (ec == CompletionError::OK) {

            ChatInfoPack pack;
            pack.fillFromBuffer(buffer);

            for (const auto& chat : pack.getVector()) {
                auto chatPtr = std::make_shared<Chat>();
                chatPtr->id = chat.getId();
                chatPtr->nickname = chat.getNickname();
                chatPtr->title = chat.getTitle();
                chatPtr->hasPassword = chat.hasPassword();
                chatPtr->messages = std::list<MessagePtr>();

                mUserChats.push_back(chatPtr);
                mLoadedChats.push_back(chatPtr);
            }

            loadUserMessages();

        }
        else {
            // TODO: Report problem!
        }

    });
}

void Container::loadUserMessages()
{
    for (const auto& chat : mUserChats) {

        loadMessages(chat->id, 0, nullptr);

    }
}

void Container::loadMessages(int chatId, time_t timeOld, loadedHandler handler)
{
    mTasker.requestChatMessage(chatId, timeOld, [=](auto ec, auto buffer){

        if (ec == CompletionError::OK) {

            ChatPtr chat = getChat(chatId);

            if (!chat)
                return; // TODO: Something more angry

            ChatMessagePack pack;
            pack.fillFromBuffer(buffer);

            for (const auto& msg : pack.getVector()) {

                auto message = std::make_shared<Message>();

                message->id = msg.getId();
                message->text = msg.getText();

                auto senderId = msg.getSenderId();
                auto author = getUser(msg.getSenderId());
                if (!author) {
                    loadUser(senderId, [message] (bool ok, UserPtr user) {
                        if (ok)
                            message->author = user;
                    });
                }
                else
                    message->author = author;

                chat->messages.push_back(message);

            }

            if (handler)
                handler();
        }
        else {
            // TODO: report problem!
        };

    });
}

ChatPtr Container::getChat(const id_t& id) const
{
    std::vector<ChatPtr>::const_iterator it;
    if (id.index() == 0) { // int
        auto _id = std::get<int>(id);
        it = std::find_if(mLoadedChats.begin(), mLoadedChats.end(), [_id](const ChatPtr& ptr) {
            return ptr->id == _id;
        });
    }
    else {
        auto name = std::get<std::string>(id);
        it = std::find_if(mLoadedChats.begin(), mLoadedChats.end(), [name] (const ChatPtr& ptr) {
            return ptr->nickname == name;
        });
    }

    if (it == mLoadedChats.end())
        return nullptr;

    return *it;
}

UserPtr Container::getUser(const id_t& id) const
{
    std::vector<UserPtr>::const_iterator it;
    if (id.index() == 0) { // int
        auto _id = std::get<int>(id);
        it = std::find_if(mLoadedUsers.begin(), mLoadedUsers.end(), [_id](const UserPtr& ptr) {
            return ptr->id == _id;
        });
    }
    else {
        auto name = std::get<std::string>(id);
        it = std::find_if(mLoadedUsers.begin(), mLoadedUsers.end(), [name] (const UserPtr& ptr) {
            return ptr->name == name;
        });
    }

    if (it == mLoadedUsers.end())
        return nullptr;

    return *it;
}

void Container::addUserChat(const ChatPtr& chat)
{
    mUserChats.push_back(chat);
    loadMessages(chat->id, 0, nullptr);
}

void Container::removeUserChat(const ChatPtr& chat)
{
    auto it =
    std::find_if(mUserChats.begin(), mUserChats.end(), [=](const ChatPtr& ptr) {
        return chat->id == ptr->id;
    });

    if (it != mUserChats.end())
        mUserChats.erase(it);
}
