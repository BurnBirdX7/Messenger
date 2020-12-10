#include "Tasker.hpp"

// Include incomplete types
#include "Client.hpp"
#include "Context.hpp"

Tasker::Tasker(Client& client)
    : mClient(client)
    , mContext(client.getContext())
{}

void Tasker::sendMessage(int chatId, const std::string& content, const CompletionHandler& handler)
{
    // std::array<ConstBuffer, 2> seq = {boost::asio::buffer(&chatId, sizeof(chatId)), boost::asio::buffer(message)};
    std::vector<ConstBuffer> seq;
    Commons::Data::BufferComposer composer(seq);
    composer
            .append(chatId)
            .append(content);

    Task msgTask(Purpose::SEND_CHAT_MSG, seq, handler);

    mClient.addTask(std::move(msgTask));
}

void Tasker::login(const std::string& login, const std::string& password, const CompletionHandler& handler)
{
    //std::array<ConstBuffer, 2> seq = {string_buffer(login), string_buffer(password)};

    std::vector<ConstBuffer> seq;
    Commons::Data::BufferComposer composer(seq);
    composer
            .append(login)
            .append(password);

    Task loginTask(Purpose::LOGIN, seq, handler);

    mClient.addTask(std::move(loginTask));
}