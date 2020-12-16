//
// Created by artem on 17.11.2020.
//

#include "TaskManager.hpp"

using namespace Commons::Network;

TaskManager::TaskManager()
    : mIdentity(-1)
    , mTaskQueue(PriorityCompare(*this))
    , mStorage(TASK_STORAGE_SIZE)
    , mTaskStorageAvailable(TASK_STORAGE_SIZE)
    , mLocked(false)
{}

void TaskManager::addTask(Task&& task)
{
    if (mLocked) {
        task.invokeCompletionHandler(Task::DECLINED_BY_MANAGER,
                                     boost::asio::buffer("Task queue is locked"));
        return;
    }

    if (mTaskStorageAvailable == 0) {
        task.invokeCompletionHandler(Task::DECLINED_BY_MANAGER,
                                     boost::asio::buffer("Task queue is full"));
        return;
    }

    --mTaskStorageAvailable;
    ++mIdentity;

    while(mStorage[mIdentity].has_value())
        ++mIdentity;

    mStorage[mIdentity].emplace(std::move(task));
}

TaskManager::TaskId TaskManager::dequeueTask()
{
    TaskId id = mTaskQueue.top();
    mTaskQueue.pop();
    return id;
}

void TaskManager::completeTask(TaskId taskId,
                               Task::ErrorCode errorCode,
                               ConstBuffer buffer)
{
    if (mStorage[taskId].has_value()) {
        mStorage[taskId].value().invokeCompletionHandler(errorCode, buffer);
        _releaseTask(taskId);
    }
}

void TaskManager::declineAllPending()
{
    while (!mTaskQueue.empty()) {
        TaskId id = dequeueTask();
        completeTask(id,
                     Task::DECLINED_BY_MANAGER,
                     boost::asio::buffer("All tasks were declined by Task Manager"));
    }
}

void TaskManager::declineAllDispatched()
{
}

MessageRepresentation TaskManager::makeMessageFromTask(TaskManager::TaskId taskId)
{
    const Task& task = mStorage[taskId].value();

    return MessageRepresentation(task.getPurpose(),
                                 taskId,
                                 boost::asio::buffer(task.getContent())
                                 );
}

Message TaskManager::makeOwningMessageFromTask(TaskId taskId) const
{
    const Task &task = mStorage[taskId].value();

    auto size = static_cast<uint32_t>(task.getContent().size());
    return Message
    {
        { size, task.getPurpose(), taskId },
        task.getContent().data(),
        size
    };
}

bool TaskManager::isEmpty() const
{
    return mTaskStorageAvailable == TASK_STORAGE_SIZE;
}

bool TaskManager::isQueueEmpty() const
{
    return mTaskQueue.empty();
}

void TaskManager::releaseTask(TaskId taskId)
{
    if (mStorage[taskId].has_value())
        _releaseTask(taskId);
}

inline void TaskManager::_releaseTask(TaskId taskId)
{
    mStorage[taskId].reset();
    ++mTaskStorageAvailable;
}

Task::Type TaskManager::getTypeOfTask(TaskManager::TaskId taskId) const
{
    return mStorage[taskId]->getType();
}

void TaskManager::releaseIfAnswer(TaskId taskId)
{
    if (mStorage[taskId].has_value() && mStorage[taskId]->getType() == Task::Type::ANSWER) {
        _releaseTask(taskId);
    }
}

bool TaskManager::isLocked() const
{
    return mLocked;
}

void TaskManager::lock()
{
    mLocked = true;
}

void TaskManager::unlock()
{
    mLocked = false;
}

TaskManager::PriorityCompare::PriorityCompare(const TaskManager &tm) noexcept
    : mParent(tm)
{}

bool TaskManager::PriorityCompare::operator()(TaskId a, TaskId b) noexcept
{
    return mParent.mStorage[a]->getPriority() < mParent.mStorage[b]->getPriority();
}