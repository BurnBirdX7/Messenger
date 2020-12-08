#ifndef MESSENGER_CHATINFO_HPP
#define MESSENGER_CHATINFO_HPP

#include "IReceivable.hpp"
#include "ISendable.hpp"

#include "Types.hpp"
#include "Buffer.hpp"
#include "BufferDecomposer.hpp"

namespace Commons::Data {

    // Information about chat
    class ChatInfo
        : public IReceivable
        , public ISendable
    {
    public:
        ChatInfo() = default;

        ConstBufferVector getConstDataSequence() const override;
        size_t bytes() const override;

        void fillFromBuffer(const ConstBuffer& buffer) override;

        [[nodiscard]] int4 getId() const;
        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] bool isDirect() const;
        [[nodiscard]] time_t getTimeUpdated() const;
        [[nodiscard]] const std::string& getNickname() const;

        void setId(int4 id);
        void setTitle(const std::string& title);
        void setIsDirect(bool isDirect);
        void setTimeUpdated(time_t timeUpdated);
        void setNickname(const std::string& nickname);

    private:

        // Id of the chat
        int4 mId;

        // Title of the chat
        std::string mTitle;

        // is this a direct chat
        bool mIsDirect;

        // last time the chat updated // TODO: clarify
        time_t mTimeUpdated;

        // Unique name of the chat
        std::string mNickname;

    };

}

#endif //MESSENGER_CHATINFO_HPP
