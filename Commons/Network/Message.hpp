#ifndef ASIOAPPLICATION_MESSAGE_HPP
#define ASIOAPPLICATION_MESSAGE_HPP

/* Message structure
 *
 *  +----------------------------------------------+
 *  |                HEADER - 6 bytes              |
 * ++====================++===========++===========++--------------------------~  ~--+
 * ||         [N]        ||           ||           ||                          /  /  |
 * ||  MESSAGE'S LENGTH  ||  PURPOSE  ||  TASK ID  ||  CONTENT OF THE MESSAGE  /  /  |
 * ||        uint32      ||   uint8   ||           ||                          /  /  |
 * ++====================++===========++===========++--------------------------~  ~--+
 *          4 bytes          1 byte       1 byte               N bytes
 *
 */

#include <cstdint>
#include <array>
#include <memory>

#include <boost/asio.hpp>

#include "NetworkTypes.hpp"
#include "IMessage.hpp"


namespace Commons::Network {

    struct MessageHeader
    {
        uint32_t length;
        uint8_t  purposeByte;
        uint8_t  taskId;

        MutableBufferArray<3> getBufferSequence();
        ConstBufferArray<3>   getBufferSequence() const;
    };

    // Safe message data storage
    class Message
            : public IMessage
    {
    public:
        using ContentPointer = std::shared_ptr<uint8_t>;

        Message(const MessageHeader&, ContentPointer);

        MessageHeader&       header();
        const MessageHeader& header() const;

        ContentPointer&       content();
        const ContentPointer& content() const;

        MutableBuffer getContentBuffer();
        ConstBuffer   getContentBuffer() const override; // also implements interface

    public: // IMessage Interface implementation
        uint8_t        getPurpose()        const override;
        uint8_t        getTaskId()         const override;
        uint32_t       getContentLength()  const override;
        const uint8_t* getContentRawData() const override; // unsafe

        ConstBufferArray<3>   getHeaderBufferSequence() const override;
        ConstBufferVector     getBufferSequence()       const override;

    private:
        MessageHeader  mHeader;
        ContentPointer mContent;

    };

}

#endif //ASIOAPPLICATION_MESSAGE_HPP
