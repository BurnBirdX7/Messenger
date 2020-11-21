//
// Created by artem on 17.11.2020.
//

#include "TaskManager.hpp"

using namespace Commons::Network;

TaskManager::TaskManager()
    : mIdentity(-1)
    , mTaskQueue(PriorityCompare(*this))
    , mStorage(TASK_STORAGE_SIZE, std::nullopt)
    , mTaskStorageAvailable(TASK_STORAGE_SIZE)
{

}

void TaskManager::addTask(Task&& task)
{
    if (mTaskStorageAvailable == 0) {
        task.invokeCompletionHandler(Task::DECLINED_BY_MANAGER,
                                     boost::asio::buffer("Task Manager has no storage"));
    }

    mIdentity++;

    for ( ; mStorage[mIdentity].has_value() ; ++mIdentity)
        ;

    mStorage[mIdentity].emplace(std::move(task));

    --mTaskStorageAvailable;
}

TaskManager::TaskId TaskManager::dequeueTask()
{
    TaskId id = mTaskQueue.top();
    mTaskQueue.pop();
    return id;
}

void TaskManager::completeTask(TaskId taskId,
                               Task::error_code_t errorCode,
                               ConstBuffer buffer)
{
    if (mStorage[taskId].has_value()) {
        mStorage[taskId].value().invokeCompletionHandler(errorCode, buffer);
        mStorage[taskId].reset();
        ++mTaskStorageAvailable;
    }
}

void TaskManager::declineAll() {

    while (!mTaskQueue.empty()) {
        TaskId id = dequeueTask();
        completeTask(id,
                     Task::DECLINED_BY_MANAGER,
                     boost::asio::buffer("All tasks were declined by Task Manager"));
    }

    mIdentity = -1;
}

MessageRepresentation TaskManager::makeMessageFromTask(TaskManager::TaskId taskId)
{
    const Task& task = mStorage[taskId].value();

    return MessageRepresentation(task.getPurpose(),
                                 taskId,
                                 boost::asio::buffer(task.getContent())
                                 );
}

bool TaskManager::isEmpty() const {
    return mTaskStorageAvailable == TASK_STORAGE_SIZE;
}

TaskManager::PriorityCompare::PriorityCompare(const TaskManager &tm) noexcept
    : mParent(tm)
{}

bool TaskManager::PriorityCompare::operator()(TaskId a, TaskId b) noexcept
{
    return mParent.mStorage[a]->getPriority() < mParent.mStorage[b]->getPriority();
}