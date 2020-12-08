#include "Buffer.hpp"

using namespace Commons::Data;

Buffer::ConstBuffer Buffer::stdString(const std::string& str)
{
    return boost::asio::buffer(str.c_str(), str.length() + 1); // +1 to capture null-byte
}

Buffer::MutableBuffer Buffer::stdString(std::string& str)
{
    return boost::asio::buffer(str);
}


