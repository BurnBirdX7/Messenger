#ifndef ASIOAPPLICATION_MESSAGEREPRESENTATION_H
#define ASIOAPPLICATION_MESSAGEREPRESENTATION_H

#include <cstdint>
#include <vector>
#include <array>
#include <boost/asio.hpp>

#include "NetworkTypes.hpp"

namespace Commons::Network {

    class MessageRepresentation {

    public: // definitions
        constexpr static uint8_t MESSAGE_API_MAJOR_VERSION = 0;
        constexpr static uint8_t MESSAGE_API_MINOR_VERSION = 1;

        MessageRepresentation(const uint8_t& purpose,
                              const uint8_t& taskId,
                              ConstBuffer content);

        MessageRepresentation(const uint8_t& purpose,
                              const uint8_t& taskId);

        [[nodiscard]]
        ConstBufferVector getBufferSequence() const;

    private:

        int32_t mLength;
        ConstBuffer mPurpose;
        ConstBuffer mTaskId;
        ConstBuffer mContent;

    };

}

#endif //ASIOAPPLICATION_MESSAGEREPRESENTATION_H
