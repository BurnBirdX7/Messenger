#ifndef MESSENGER_SESSIONINFO_HPP
#define MESSENGER_SESSIONINFO_HPP

#include "IReceivable.hpp"
#include "ISendable.hpp"
#include "Types.hpp"

#include "BufferDecomposer.hpp"
#include "BufferComposer.hpp"

namespace Commons::Data {

    class SessionInfo
        : public IReceivable
        , public ISendable
    {
    public:
        SessionInfo() = default;

        size_t fillFromBuffer(const ConstBuffer& buffer) override;

        ConstBufferVector getConstDataSequence() const override;
        size_t bytes() const override;

        [[nodiscard]] int4 getId() const;
        [[nodiscard]] int4 getIp4() const;
        [[nodiscard]] time_t getLastActivity() const;
        [[nodiscard]] int4 getUserId() const;
        [[nodiscard]] const std::string& getHash() const;

        void setId(int4 id);
        void setIp4(int4 ip4);
        void setLastActivity(time_t lastActivity);
        void setUserId(int4 userId);
        void setHash(const std::string& hash);

    private:
        int4 mId{};
        int4 mIp4{};
        time_t mLastActivity{};
        int4 mUserId{};

        // Won't be sent:
        std::string mHash;

    };

}


#endif //MESSENGER_SESSIONINFO_HPP
