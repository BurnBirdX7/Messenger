#ifndef MESSENGER_IRECEIVABLE_HPP
#define MESSENGER_IRECEIVABLE_HPP

#include "Types.hpp"

namespace Commons::Data {

    class IReceivable
    {
    public:
        using ConstBuffer = boost::asio::const_buffer;

        virtual void fillFromBuffer(const ConstBuffer& buffer) = 0;

    };



}

#endif //MESSENGER_IRECEIVABLE_HPP
