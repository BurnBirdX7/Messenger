#ifndef ASIOAPPLICATION_TASK_HPP
#define ASIOAPPLICATION_TASK_HPP

#include <cstdint>
#include <utility>
#include <vector>
#include <functional>

#include <boost/asio.hpp>
#include <optional>

#include "NetworkTypes.hpp"

namespace Commons::Network {

    /*
     * class Task
     * represents message to be sent (it's purpose and content) and it's priority
     *
     * Task is not copyable, but is movable
     */
    class Task {
    public: // definitions
        enum class Priority : uint8_t {
            LOW    = 000,
            LOWER  = 063,
            MEDIUM = 127,
            HIGHER = 191,
            HIGH   = 255,
        };

        enum Type : bool {
            ANSWER  = false,
            REQUEST = true
        };

        enum ErrorCode : int {
            OK                   = 0,
            DECLINED_BY_RECEIVER = 1,
            DECLINED_BY_MANAGER  = 2,
            CONNECTION_ERROR     = 3,
        };

        using MessageContentContainer = std::vector<uint8_t>;
        using CompletionHandler = std::function<void(ErrorCode, ConstBuffer)>;

    public: // static helper methods
        static Task createHelloTask(const CompletionHandler&);
        static Task createDisconnectTask(const CompletionHandler&);

    public: // methods
        // Constructs REQUEST task with content
        template <class ConstBufferSequence>
        Task(uint8_t purpose,
             const ConstBufferSequence& bufferSequence,
             CompletionHandler completionHandler,
             Task::Priority priority = Priority::MEDIUM);

        // Constructs REQUEST task without content
        Task(uint8_t purpose,
             CompletionHandler completionHandler,
             Task::Priority priority = Priority::MEDIUM);

        // Constructs ANSWER task with content
        template <class ConstBufferSequence>
        Task(uint8_t purpose,
             const ConstBufferSequence& bufferSequence,
             uint8_t requestedTaskId,
             Task::Priority priority = Priority::MEDIUM);

        // Constructs ANSWER task without content
        Task (uint8_t purpose,
              uint8_t requestedTaskId,
              Task::Priority priority = Priority::MEDIUM);

        // Default move constructor and move assignment operator
        Task(Task&&) = default;
        Task& operator=(Task&&) = default;

        // Deleted copy constructor and copy assignment operator
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        template <class ConstBufferSequence>
        void setContent(const ConstBufferSequence&);

        [[nodiscard]] Type getType() const;
        [[nodiscard]] Priority getPriority() const;
        [[nodiscard]] uint8_t getPurpose() const;
        [[nodiscard]] const MessageContentContainer& getContent() const;

        void invokeCompletionHandler(ErrorCode error_code, ConstBuffer);

    private: // methods
        template <class ConstBufferSequence>
        void copyContent(const ConstBufferSequence& sequence);

    private: // fields
        Priority mPriority;
        uint8_t mPurpose;
        MessageContentContainer mContent;

        std::optional<CompletionHandler> mCompletionHandler;
        std::optional<uint8_t> mTaskAnswerId;

    };


    /* Template Methods Declaration: */

    template<class ConstBufferSequence>
    Task::Task(uint8_t purpose,
             const ConstBufferSequence& bufferSequence,
             CompletionHandler completionHandler,
             Task::Priority priority)
        : mPriority(priority)
        , mPurpose(purpose)
        , mCompletionHandler(std::in_place, std::move(completionHandler))
        , mContent(0)
        , mTaskAnswerId(std::nullopt)
    {
        copyContent(bufferSequence);
    }

    template <class ConstBufferSequence>
    Task::Task(uint8_t purpose,
               const ConstBufferSequence& bufferSequence,
               uint8_t requestedTaskId,
               Task::Priority priority)
        : mPriority(priority)
        , mPurpose(purpose)
        , mCompletionHandler(std::nullopt)
        , mContent(0)
        , mTaskAnswerId(requestedTaskId)
    {
        copyContent(bufferSequence);
    }

    template<class ConstBufferSequence>
    void Task::setContent(const ConstBufferSequence& sequence)
    {
        copyContent(sequence);
    }

    template <class ConstBufferSequence>
    void Task::copyContent(const ConstBufferSequence& sequence)
    {
        auto seq_begin = boost::asio::buffer_sequence_begin(sequence);
        auto seq_end = boost::asio::buffer_sequence_end(sequence);

        size_t size = 0;
        for (auto seq_it = seq_begin; seq_it != seq_end; ++seq_it)
            size += seq_it->size();

        mContent.resize(size);
        auto contentBuffer = boost::asio::buffer(mContent);

        boost::asio::buffer_copy(contentBuffer, sequence);
    }

}

#endif //ASIOAPPLICATION_TASK_HPP
