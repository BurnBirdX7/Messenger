#ifndef MESSENGER_TASKER_HPP
#define MESSENGER_TASKER_HPP

#include <string>

#include <Network/Task.hpp>
#include <Data/Types.hpp>
#include <Data/BufferComposer.hpp>
#include <Data/BufferDecomposer.hpp>

class Client;
class Context;

class Tasker
{
public:
    using ConstBuffer = Commons::Data::ConstBuffer;

    using Task = Commons::Network::Task;
    using CompletionHandler = Task::CompletionHandler;

    using Purpose = Commons::Network::Purpose;

public:
    explicit Tasker(Client&);

    void login(const std::string& login, const std::string& password, const CompletionHandler& handler);
    void sendMessage(int chatId, const std::string& content, const CompletionHandler& handler);

private:
    Client& mClient;
    Context& mContext;

};


#endif //MESSENGER_TASKER_HPP
