#include "Buffer.hpp"

using namespace Commons::Data;

Buffer::ConstBuffer Buffer::string(const std::string& str)
{
    return boost::asio::buffer(str.c_str(), str.length() + 1); // +1 to capture null-byte
}

Buffer::ConstBuffer Buffer::string(const char* str)
{
    return boost::asio::buffer(str, std::strlen(str) + 1); // +1 to capture null-byte
}




