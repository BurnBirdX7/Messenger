#ifndef MESSENGER_TYPES_HPP
#define MESSENGER_TYPES_HPP

#include <cstdint>

#include <boost/asio.hpp>

namespace Commons::Data {

    using ConstBuffer = boost::asio::const_buffer;
    using MutableBuffer = boost::asio::mutable_buffer;

    using int1 = int8_t;
    using int2 = int16_t;
    using int4 = int32_t;
    using int8 = int64_t;

}

#endif //MESSENGER_TYPES_HPP
