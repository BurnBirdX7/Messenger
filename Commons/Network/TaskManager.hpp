//
// Created by artem on 17.11.2020.
//

#ifndef ASIOAPPLICATION_TASKMANAGER_HPP
#define ASIOAPPLICATION_TASKMANAGER_HPP

#include <cstdint>
#include <array>
#include <queue>
#include <optional>
#include <variant>

#include "Task.hpp"
#include "MessageRepresentation.hpp"

namespace Commons::Network {

    class TaskManager
    {
    public: // definitions

        constexpr static int TASK_STORAGE_SIZE = 256;
        using TaskId = uint8_t;

    public: // methods

        TaskManager();

        void addTask(Task&&);

        [[nodiscard]]
        TaskId dequeueTask();

        MessageRepresentation makeMessageFromTask(TaskId);

        void releaseTask(TaskId);
        void completeTask(TaskId, Task::ErrorCode, ConstBuffer);
        Task::Type getTypeOfTask(TaskId) const;
        void releaseIfAnswer(TaskId);

        void declineAll();

        bool isEmpty() const;

    private: // methods
        class PriorityCompare
        {
        public:
            explicit PriorityCompare(const TaskManager& tm) noexcept;
            inline bool operator() (TaskId a, TaskId b) noexcept;
        private:
            const TaskManager& mParent;
        };

        void _releaseTask(TaskId);

    private: // fields
        using TaskQueue = std::priority_queue<TaskId, std::vector<TaskId>, PriorityCompare>;
        using TaskStorage = std::vector<std::optional<Task>>;

        uint16_t mTaskStorageAvailable;
        uint8_t mIdentity;
        TaskStorage mStorage;
        TaskQueue mTaskQueue;

    };

}

#endif //ASIOAPPLICATION_TASKMANAGER_HPP
