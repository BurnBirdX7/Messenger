#ifndef MESSENGER_CHATMESSAGE_HPP
#define MESSENGER_CHATMESSAGE_HPP

#include "ISendable.hpp"
#include "IReceivable.hpp"
#include "PrimitiveTypes.hpp"

namespace Commons::Data {

    class ChatMessage
            : public ISendable
            , public IReceivable
    {
    public:
        ChatMessage() = default;

        ConstBufferVector getConstDataSequence() const override;
        MutableBufferVector getMutableDataSequence() override;

        [[nodiscard]] int4 getSenderId() const;
        [[nodiscard]] int4 getChatId() const;
        [[nodiscard]] const std::string& getText() const;
        [[nodiscard]] time_t getTimeCreated() const;
        [[nodiscard]] time_t getTimeUpdated() const;
        [[nodiscard]] int1 getStatus() const;

        void setSenderId(int4 sender);
        void setChatId(int4 chatId);
        void setText(const std::string& text);
        void setTimeCreated(time_t timeCreated);
        void setTimeUpdated(time_t timeUpdated);
        void setStatus(int1 status);

    private:
        int4        mSenderId{};
        int4        mChatId{};
        std::string mText;
        time_t      mTimeCreated{};
        time_t      mTimeUpdated{};
        int1        mStatus{};

    };

}

#endif //MESSENGER_CHATMESSAGE_HPP
