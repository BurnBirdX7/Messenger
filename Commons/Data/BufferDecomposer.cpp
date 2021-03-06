#include "BufferDecomposer.hpp"

using namespace Commons::Data;

BufferDecomposer::BufferDecomposer(const ConstBuffer& buffer)
        : mBuffer(buffer)
{}

size_t BufferDecomposer::bytesLeft() const
{
    return mBuffer.size();
}