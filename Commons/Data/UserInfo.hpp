#ifndef MESSENGER_USERINFO_HPP
#define MESSENGER_USERINFO_HPP

#include "Types.hpp"
#include "IReceivable.hpp"
#include "ISendable.hpp"

#include "BufferComposer.hpp"
#include "BufferDecomposer.hpp"

namespace Commons::Data {

    // Represents public user info
    class UserInfo
        : public IReceivable
        , public ISendable
    {
    public:
        UserInfo() = default;

        ConstBufferVector getConstDataSequence() const override;
        size_t bytes() const override;

        size_t fillFromBuffer(const ConstBuffer& buffer) override;

        [[nodiscard]] int4 getId() const;
        [[nodiscard]] const std::string& getName() const;
        [[nodiscard]] const std::string& getNickname() const;
        [[nodiscard]] const std::string& getPasswordHash() const;

        void setId(int4 id);
        void setName(const std::string& name);
        void setNickname(const std::string& nickname);
        void setPasswordHash(const std::string& passwordHash);

    private:
        int4 mId{};
        std::string mName;
        std::string mNickname;

        // WON'T be sent:
        std::string mPasswordHash;
    };

}

#endif //MESSENGER_USERINFO_HPP
