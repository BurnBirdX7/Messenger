#ifndef ASIOAPPLICATION_TASKMANAGER_HPP
#define ASIOAPPLICATION_TASKMANAGER_HPP

#include <cstdint>
#include <array>
#include <queue>
#include <optional>
#include <variant>

#include "Task.hpp"
#include "MessageRepresentation.hpp"
#include "Message.hpp"

namespace Commons::Network {

    // Isn't thread-safe
    class TaskManager
    {
    public:
        constexpr static int TASK_STORAGE_SIZE = 256;
        using TaskId = uint8_t;

    public:

        TaskManager();

        // Adds task to the queue
        void addTask(Task&&);

        // Extracts task from queue
        [[nodiscard]] TaskId dequeueTask();

        // Makes not-owning message (data will be referenced from task)
        MessageRepresentation makeMessageFromTask(TaskId);

        // Makes owning message (data will be copied)
        [[nodiscard]] Message makeOwningMessageFromTask(TaskId) const;

        // Releases task
        void releaseTask(TaskId);

        // Invokes completion handler of the task
        void completeTask(TaskId, Task::ErrorCode, ConstBuffer);

        // Returns Task::Type of the task
        [[nodiscard]] Task::Type getTypeOfTask(TaskId) const;

        // Releases task if Type of the task is Task::Type::ANSWER
        void releaseIfAnswer(TaskId);

        // Declines all pending tasks
        // Doesn't decline tasks which already have been dispatched
        void declineAllPending();

        // Declines all already dispatched tasks with CONNECTION_ERROR error code
        void declineAllDispatched();

        // If manager is locked it won't accept any new tasks
        // Unlocked by default
        [[nodiscard]] bool isLocked() const;

        // Locks manager
        void lock();

        // Unlocks manager
        void unlock();

        // Checks if task storage is empty
        [[nodiscard]] bool isEmpty() const;

        // Checks if task queue is empty
        [[nodiscard]] bool isQueueEmpty() const;

    private:
        // Comparator class
        class PriorityCompare
        {
        public:
            explicit PriorityCompare(const TaskManager& tm) noexcept;
            inline bool operator() (TaskId a, TaskId b) noexcept;

        private:
            const TaskManager& mParent;

        };

        void _releaseTask(TaskId);

    private:
        using TaskQueue = std::priority_queue<TaskId, std::vector<TaskId>, PriorityCompare>;
        using TaskStorage = std::vector<std::optional<Task>>;

        uint16_t mTaskStorageAvailable;
        uint8_t mIdentity;
        TaskStorage mStorage;
        TaskQueue mTaskQueue;
        bool mLocked;

    };

}

#endif //ASIOAPPLICATION_TASKMANAGER_HPP
