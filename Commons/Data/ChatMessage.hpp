#ifndef MESSENGER_CHATMESSAGE_HPP
#define MESSENGER_CHATMESSAGE_HPP

#include "ISendable.hpp"
#include "IReceivable.hpp"
#include "Types.hpp"
#include "Buffer.hpp"
#include "BufferDecomposer.hpp"

namespace Commons::Data {

    class ChatMessage
            : public ISendable
            , public IReceivable
    {
    public:
        ChatMessage() = default;

        // ISendable
        ConstBufferVector getConstDataSequence() const override;
        size_t bytes() const override;

        // IReceivable
        void fillFromBuffer(const ConstBuffer& buffer) override;

        // Getters
        [[nodiscard]] int4 getId() const;
        [[nodiscard]] int4 getSenderId() const;
        [[nodiscard]] int4 getChatId() const;
        [[nodiscard]] const std::string& getText() const;
        [[nodiscard]] time_t getTimeCreated() const;
        [[nodiscard]] time_t getTimeUpdated() const;
        [[nodiscard]] int1 getStatus() const;

        // Setters
        void setId(int4 id);
        void setSenderId(int4 senderId);
        void setChatId(int4 chatId);
        void setText(const std::string& text);
        void setTimeCreated(time_t timeCreated);
        void setTimeUpdated(time_t timeUpdated);
        void setStatus(int1 status);

    private:
        // ID of the message
        int4        mId{};

        // ID of the sender (user)
        int4        mSenderId{};

        // ID of the chat where message was sent
        int4        mChatId{};

        // Time of creation (sent)
        time_t      mTimeCreated{};

        // Time of update (edit)
        time_t      mTimeUpdated{};

        // ID of a status (sent, seen, deleted) of the message
        int1        mStatus{};

        // Content of the message
        std::string mText;

    };

}

#endif //MESSENGER_CHATMESSAGE_HPP
