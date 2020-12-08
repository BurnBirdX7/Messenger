#ifndef MESSENGER_BUFFERDECOMPOSER_HPP
#define MESSENGER_BUFFERDECOMPOSER_HPP

#include "Buffer.hpp"

namespace Commons::Data {

    class BufferDecomposer
    {
    public:
        using ConstBuffer = Buffer::ConstBuffer;

    public:
        explicit BufferDecomposer(const ConstBuffer& buffer)
            : mBuffer(buffer)
        {}

        template <class Type>
        Type get();

        // It's important to check if
        size_t bytesLeft();

    private:
        ConstBuffer mBuffer;

    };

    // For primitive types
    template <class Type>
    Type BufferDecomposer::get()
    {
        size_t bytes = sizeof(Type);
        Type var = *static_cast<const Type*>(mBuffer.data());
        mBuffer += bytes;
        return var;
    }

    size_t BufferDecomposer::bytesLeft()
    {
        return mBuffer.size();
    }

    template <>
    std::string BufferDecomposer::get()
    {
        auto c_str = static_cast<const char*>(mBuffer.data()); // Until meet null-byte
        std::string str(c_str);
        mBuffer += str.length() + 1; // +1 to capture null-byte
        return str;
    }



}

#endif //MESSENGER_BUFFERDECOMPOSER_HPP
