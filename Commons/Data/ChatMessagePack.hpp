#ifndef MESSENGER_CHATMESSAGEPACK_HPP
#define MESSENGER_CHATMESSAGEPACK_HPP

#include <vector>

#include "IReceivable.hpp"
#include "ISendable.hpp"

#include "ChatMessage.hpp"

namespace Commons::Data {

    class ChatMessagePack
        : public IReceivable
        , public ISendable
    {
    public:
        ChatMessagePack() = default;
        explicit ChatMessagePack(size_t count);

        // ISendable
        ConstBufferVector getConstDataSequence() const override;
        size_t bytes() const override;

        // IReceivable
        size_t fillFromBuffer(const ConstBuffer& buffer) override;

        void addMessage(const ChatMessage& message);
        const ChatMessage& at(size_t index) const;
        size_t count() const;

        const std::vector<ChatMessage>& getVector() const;

    private:
        std::vector<ChatMessage> mPack;

    };

}

#endif //MESSENGER_CHATMESSAGEPACK_HPP
