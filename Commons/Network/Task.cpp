//
// Created by artem on 17.11.2020.
//

#include "Task.hpp"

using namespace Commons::Network;


Task::Task(uint8_t purpose,
           CompletionHandler completionHandler,
           uint8_t priority)
    : mPriority(priority)
    , mPurpose(purpose)
    , mCompletionHandler(std::in_place, std::move(completionHandler))
    , mContent(0)
    , mTaskAnswerId(std::nullopt)
{}

Task::Task (uint8_t purpose,
            uint8_t requestedTaskId,
            uint8_t priority)
    : mPriority(priority)
    , mPurpose(purpose)
    , mCompletionHandler(std::nullopt)
    , mContent(0)
    , mTaskAnswerId(requestedTaskId)
{}

Task::Type Task::getType() const
{
    return mCompletionHandler.has_value() ? Type::REQUEST : Type::ANSWER;
}

uint8_t Task::getPriority() const
{
    return this->mPriority;
}

uint8_t Task::getPurpose() const
{
    return this->mPurpose;
}

const Task::MessageContentContainer& Task::getContent() const
{
    return this->mContent;
}

void Task::invokeCompletionHandler(ErrorCode error_code, ConstBuffer container)
{
    if (mCompletionHandler.has_value())
        std::invoke(mCompletionHandler.value(), error_code, container);
    else
        throw std::runtime_error("You can't invoke completion handler of ANSWER task");
}

Task Task::createHelloTask(const CompletionHandler& handler)
{
    constexpr std::array<uint8_t, 2> ver = {MESSAGE_API_MAJOR_VERSION, MESSAGE_API_MINOR_VERSION};

    return Task(Purpose::HELLO,
                boost::asio::buffer(ver),
                handler,
                Task::Priority::HIGH);
}

Task Task::createDisconnectTask(const Task::CompletionHandler& handler)
{
    return Task(Purpose::DISCONNECT,
                handler,
                Task::Priority::HIGH);
}