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
#include "MessageRepresentation.h"

namespace Commons::Network {

    class TaskManager
    {
    public: // definitions

        constexpr static uint8_t TASK_STORAGE_SIZE = 255;

        using task_id_t = uint8_t;

    public: // methods

        TaskManager();

        void addTask(Task);

        [[nodiscard]]
        task_id_t dequeueTask();

        MessageRepresentation makeMessageFromTask(task_id_t);

        void completeTask(task_id_t,
                          Task::error_code_t,
                          ConstBuffer);

        void declineAll();

        bool isEmpty() const;

    private: // methods

        class PriorityCompare
        {
        public:
            explicit PriorityCompare(const TaskManager& tm) noexcept;
            inline bool operator() (task_id_t a, task_id_t b) noexcept;
        private:
            const TaskManager& mParent;
        };

    private: // fields

        using TaskQueue = std::priority_queue<task_id_t, std::vector<task_id_t>, PriorityCompare>;
        using TaskStorage = std::vector<std::optional<Task>>;
        using TaskPriorityStorage = std::vector<uint8_t>;

        uint16_t mTaskStorageAvailable;
        uint8_t mIdentity;
        TaskStorage mStorage;
        TaskQueue mTaskQueue;

    };

}

#endif //ASIOAPPLICATION_TASKMANAGER_HPP
