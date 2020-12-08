#ifndef MESSENGER_BUFFER_HPP
#define MESSENGER_BUFFER_HPP

#include <type_traits>
#include <string>

#include <boost/asio.hpp>

namespace Commons::Data {

    class Buffer
    {
    public:
        using ConstBuffer   = boost::asio::const_buffer;
        using MutableBuffer = boost::asio::mutable_buffer;

    public:
        // Creates buffer as null-terminated string
        static ConstBuffer   stdString(const std::string& str);

        // Creates buffer which doesn't include null-byte
        static MutableBuffer stdString(std::string& str);

        template <class Type>
        static MutableBuffer primitiveType(Type& var);

        template <class Type>
        static ConstBuffer primitiveType(const Type& var);

    };

    template <class Type>
    Buffer::MutableBuffer Buffer::primitiveType(Type& var)
    {
        void* ptr = &var;
        size_t bytes = sizeof(Type);

        return MutableBuffer {ptr, bytes};
    }

    template <class Type>
    Buffer::ConstBuffer Buffer::primitiveType(const Type& var)
    {
        const void* ptr = &var;
        size_t bytes = sizeof(Type);

        return ConstBuffer {ptr, bytes};
    }

}

#endif //MESSENGER_BUFFER_HPP
