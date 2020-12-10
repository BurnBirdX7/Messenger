#ifndef MESSENGER_BUFFERCOMPOSER_HPP
#define MESSENGER_BUFFERCOMPOSER_HPP

#include <string>

#include "Types.hpp"
#include "Buffer.hpp"
#include "ISendable.hpp"

namespace Commons::Data {

    // Class which helps compose buffer sequences
    class BufferComposer
    {
    public:
        using ConstBufferVector = std::vector<ConstBuffer>;

    public:
        // Creates Composer which owns underlying vector
        BufferComposer();

        // Creates which doesn't own vector
        explicit BufferComposer(ConstBufferVector& vector);

        ~BufferComposer();

        ConstBufferVector& getVector();

        // Adds var of specified type to vector
        template <class Type>
        void add(const Type& var);

        // Chain function
        template <class Type>
        BufferComposer& append(const Type& var);


    private:
        ConstBufferVector* mVector;
        bool mOwner;
    };

    template <class Type>
    inline void BufferComposer::add(const Type& var)
    {
        mVector->push_back(Buffer::primitiveType(var));
    }

    template <>
    inline void BufferComposer::add<std::string>(const std::string& var)
    {
        mVector->push_back(Buffer::stdString(var));
    }

    template <>
    inline void BufferComposer::add<ISendable::ConstBufferVector>(const ISendable::ConstBufferVector& var)
    {
        for (const auto& item: var)
            mVector->push_back(item);
    }


    template <class Type>
    inline BufferComposer& BufferComposer::append(const Type& var)
    {
        this->add<Type>(var);
        return *this;
    }


}

#endif //MESSENGER_BUFFERCOMPOSER_HPP
