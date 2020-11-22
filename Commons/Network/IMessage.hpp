#ifndef ASIOAPPLICATION_IMESSAGE_HPP
#define ASIOAPPLICATION_IMESSAGE_HPP

#include <cstdint>

#include "NetworkTypes.hpp"

namespace Commons::Network {

    /* IMessage class
     * Interface for classes which represent Messages
     */
    class IMessage {
    public:
        virtual uint8_t        getPurpose()        const = 0;
        virtual uint8_t        getTaskId()         const = 0;
        virtual uint32_t       getContentLength()  const = 0;
        virtual const uint8_t* getContentRawData() const = 0; // unsafe

        virtual ConstBufferArray<3>   getHeaderBufferSequence() const = 0;
        virtual ConstBuffer           getContentBuffer()        const = 0;
        virtual ConstBufferVector     getBufferSequence()       const = 0;

    };

}

#endif //ASIOAPPLICATION_IMESSAGE_HPP
