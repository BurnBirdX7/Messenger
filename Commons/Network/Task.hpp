#ifndef ASIOAPPLICATION_TASK_HPP
#define ASIOAPPLICATION_TASK_HPP

/* Task structure
 *
 * +------------+------------------------+------------------~  ~-+
 * |  Priority  |  MessageRepresentation Purpose Byte  | MessageRepresentation Content  /  / |
 * +------------+------------------------+------------------~  ~-+
 *
 */

#include <cstdint>
#include <utility>
#include <vector>
#include <functional>

#include <boost/asio.hpp>

#include "NetworkTypes.hpp"

namespace Commons::Network {

    class Task {
    public:
        enum Priority : uint8_t
        {
            LOW     = 0,   //  0
            LOWER   = 63,  //  256 / 4           - 1
            MEDIUM  = 127, //  256 / 2           - 1
            HIGHER  = 191, //  256 / 2 + 256 / 4 - 1
            HIGH    = 255, //  256               - 1
        };

        enum error_codes : int
        {
            OK = 0,
            DECLINED_BY_SERVER = 1,
            DECLINED_BY_MANAGER = 2,
            CONNECTION_ERROR = 3,
        };


        using error_code_t = int;
        using MessageContentContainer = std::vector<uint8_t>;

        using CompletionHandler = std::function<void(error_code_t, ConstBuffer)>;

        struct Comparator
        {
            bool operator()(const Task& a, const Task& b)
            {
                return a.mPriority < b.mPriority;
            }
        };

    public: // methods

        template <class ConstBufferSequence>
        Task(uint8_t purposeByte,
             const ConstBufferSequence& bufferSequence,
             CompletionHandler completionHandler,
             uint8_t priority = MEDIUM);

        Task(uint8_t purposeByte,
             CompletionHandler completionHandler,
             uint8_t priority = MEDIUM);

        Task(const Task&) = default;
        Task(Task&&) = default;

        template <class ConstBufferSequence>
        void setContent(const ConstBufferSequence&);

        [[nodiscard]]
        uint8_t getPriority() const;

        [[nodiscard]]
        uint8_t getPurpose() const;

        [[nodiscard]]
        const MessageContentContainer& getContent() const;

        void invokeCompletionHandler(error_code_t error_code, ConstBuffer);

    private: // methods

        template <class ConstBufferSequence>
        void copyContent(const ConstBufferSequence& sequence);

    private: // fields

        uint8_t mPriority;
        uint8_t mPurpose;
        std::vector<uint8_t> mContent;

        CompletionHandler mCompletionHandler; // void handler(int error_code); // TODO: error_code class

    };

    // template methods declaration:

    template<class ConstBufferSequence>
    Task::Task(uint8_t purposeByte,
    const ConstBufferSequence& bufferSequence,
            CompletionHandler completionHandler,
            uint8_t priority)
    : mPriority(priority)
    , mPurpose (purposeByte)
    , mCompletionHandler(std::move(completionHandler))
    , mContent(0)
    {
        copyContent(bufferSequence);
    }

    template<class ConstBufferSequence>
    void Task::setContent(const ConstBufferSequence& sequence) {
        copyContent(sequence);
    }

    template <class ConstBufferSequence>
    void Task::copyContent(const ConstBufferSequence& sequence)
    {
        auto seq_begin = boost::asio::buffer_sequence_begin(sequence);
        auto seq_end = boost::asio::buffer_sequence_end(sequence);

        size_t to_reserve = 0;
        for (auto seq_it = seq_begin; seq_it != seq_end; ++seq_it)
            to_reserve += seq_it->size();

        if (!mContent.empty()) {
            to_reserve -= mContent.size();
            mContent.clear();
        }
        mContent.reserve(to_reserve);

        for (auto seq_it = seq_begin; seq_it != seq_end; ++seq_it) {
            auto buff_begin = boost::asio::buffers_begin(*seq_it);
            auto buff_end = boost::asio::buffers_end(*seq_it);

            for (auto buff_it = buff_begin; buff_it != buff_end; ++buff_it)
                mContent.push_back( static_cast<uint8_t>(*buff_it) );
        }

    }

}





#endif //ASIOAPPLICATION_TASK_HPP
