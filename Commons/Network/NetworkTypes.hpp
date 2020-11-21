#ifndef ASIOAPPLICATION_NETWORKTYPES_HPP
#define ASIOAPPLICATION_NETWORKTYPES_HPP

#include <cstdint>
#include <array>
#include <vector>

#include <boost/asio.hpp>

namespace Commons::Network {

    using MutableBuffer = boost::asio::mutable_buffer;
    using ConstBuffer = boost::asio::const_buffer;

    template <size_t N>
    using MutableBufferArray = std::array<MutableBuffer, N>;
    template <size_t N>
    using ConstBufferArray = std::array<ConstBuffer, N>;

    using MutableBufferVector = std::vector<MutableBuffer>;
    using ConstBufferVector = std::vector<ConstBuffer>;

    using tcp = boost::asio::ip::tcp;
}

#endif //ASIOAPPLICATION_NETWORKTYPES_HPP
