#ifndef MESSENGER_IRECEIVABLE_HPP
#define MESSENGER_IRECEIVABLE_HPP

#include "Types.hpp"

namespace Commons::Data {

    class IReceivable
    {
    public:
        using ConstBuffer = boost::asio::const_buffer;

        // Returns amount of bytes used  to fill the object
        virtual size_t fillFromBuffer(const ConstBuffer& buffer) = 0;

    };



}

#endif //MESSENGER_IRECEIVABLE_HPP
