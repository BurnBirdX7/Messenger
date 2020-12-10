#ifndef MESSENGER_BUFFERDECOMPOSER_HPP
#define MESSENGER_BUFFERDECOMPOSER_HPP

#include "Buffer.hpp"

namespace Commons::Data {

    class BufferDecomposer
    {
    public:
        using ConstBuffer = Buffer::ConstBuffer;

    public:
        explicit BufferDecomposer(const ConstBuffer& buffer);

        template <class Type>
        Type get();

        template <class Type>
        BufferDecomposer& extract(Type&);

        size_t bytesLeft() const;

    private:
        ConstBuffer mBuffer;

    };

    // For primitive types
    template <class Type>
    inline Type BufferDecomposer::get()
    {
        size_t bytes = sizeof(Type);

        if (bytes > bytesLeft())
            throw std::runtime_error("Buffer is too small");

        Type var = *static_cast<const Type*>(mBuffer.data());
        mBuffer += bytes;
        return var;
    }

    template <>
    inline std::string BufferDecomposer::get<std::string>()
    {
        auto c_str = static_cast<const char*>(mBuffer.data()); // Until meet null-byte
        std::string str(c_str);
        mBuffer += str.length() + 1; // +1 to capture null-byte
        return str;
    }


    template <class Type>
    inline BufferDecomposer& BufferDecomposer::extract(Type& var)
    {
        var = get<Type>();
        return *this;
    }



}

#endif //MESSENGER_BUFFERDECOMPOSER_HPP
