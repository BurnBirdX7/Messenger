//
// Created by artem on 17.11.2020.
//

#include "Task.hpp"

using namespace Commons::Network;


Task::Task(uint8_t purposeByte,
           CompletionHandler completionHandler,
           uint8_t priority)
    : mPriority(priority)
    , mPurpose(purposeByte)
    , mCompletionHandler(std::move(completionHandler))
    , mContent(0)
{}

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

void Task::invokeCompletionHandler(error_code_t error_code, ConstBuffer container)
{
    std::invoke(this->mCompletionHandler, error_code, container);
}
