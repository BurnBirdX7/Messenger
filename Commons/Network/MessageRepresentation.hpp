#ifndef ASIOAPPLICATION_MESSAGEREPRESENTATION_HPP
#define ASIOAPPLICATION_MESSAGEREPRESENTATION_HPP

#include <cstdint>
#include <vector>
#include <array>
#include <boost/asio.hpp>

#include "NetworkTypes.hpp"

#include "IMessage.hpp"

namespace Commons::Network {

    // Just represents message data.
    // You have to provide guarantee that all the passed data are viable until all supposed operations are over.
    // Note that operations can be asynchronous.
    class MessageRepresentation
            : public IMessage
    {
    public: // definitions
        MessageRepresentation(const uint8_t& purpose,
                              const uint8_t& taskId,
                              ConstBuffer content);

        MessageRepresentation(const uint8_t& purpose,
                              const uint8_t& taskId);

        [[nodiscard]]
        ConstBufferVector getBufferSequence() const override; // Also implements interface

    public: // IMessage Interface implementation
        [[nodiscard]] uint8_t        getPurpose()        const override;
        [[nodiscard]] uint8_t        getTaskId()         const override;
        [[nodiscard]] uint32_t       getContentLength()  const override;
        [[nodiscard]] const uint8_t* getContentRawData() const override; // unsafe

        [[nodiscard]] ConstBufferArray<3> getHeaderBufferSequence() const override;
        [[nodiscard]] ConstBuffer         getContentBuffer()        const override;

    private:
        int32_t mLength;
        ConstBuffer mPurpose;
        ConstBuffer mTaskId;
        ConstBuffer mContent;

    };

}

#endif //ASIOAPPLICATION_MESSAGEREPRESENTATION_HPP
