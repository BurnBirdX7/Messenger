#ifndef MESSENGER_CHATMESSAGEPACK_HPP
#define MESSENGER_CHATMESSAGEPACK_HPP

#include <vector>

#include "IReceivable.hpp"
#include "ISendable.hpp"

#include "ChatInfo.hpp."

namespace Commons::Data {

    class ChatInfoPack
            : public IReceivable
              , public ISendable
    {
    public:
        ChatInfoPack() = default;
        explicit ChatInfoPack(size_t count);

        // ISendable
        ConstBufferVector getConstDataSequence() const override;
        size_t bytes() const override;

        // IReceivable
        size_t fillFromBuffer(const ConstBuffer& buffer) override;

        void addChat(const ChatInfo& chat);
        const ChatInfo& at(size_t index) const;
        size_t count() const;

        const std::vector<ChatInfo>& getVector() const;

    private:
        std::vector<ChatInfo> mPack;

    };

}

#endif //MESSENGER_CHATMESSAGEPACK_HPP
