#ifndef MESSENGER_BUFFERCOMPOSER_HPP
#define MESSENGER_BUFFERCOMPOSER_HPP

#include "Types.hpp"
#include "Buffer.hpp"

namespace Commons::Data {

    // Class which helps compose buffer sequences
    class BufferComposer
    {
    public:
        using ConstBufferVector = std::vector<ConstBuffer>;

    public:
        // Creates Composer which own underlying vector
        inline BufferComposer()
            : mVector(new ConstBufferVector())
            , mOwner(true)
        {}

        // Creates which doesn't own vector
        inline explicit BufferComposer(ConstBufferVector& vector)
            : mVector(&vector)
            , mOwner(false)
        {}

        inline ~BufferComposer()
        {
            if (mOwner) // Deletes vector only if owns it
                delete mVector;
        }

        inline ConstBufferVector& getVector()
        {
            return *mVector;
        }

        template <class Type>
        void add(const Type& var);

        template <class Type>
        BufferComposer& append(const Type& var);


    private:
        ConstBufferVector* mVector;
        bool mOwner;
    };

    template <class Type>
    void BufferComposer::add(const Type& var)
    {
        mVector->push_back(Buffer::primitiveType(var));
    }

    template <>
    void BufferComposer::add(const std::string& var)
    {
        mVector->push_back(Buffer::stdString(var));
    }

    template <class Type>
    BufferComposer& BufferComposer::append(const Type& var)
    {
        this->add<Type>(var);
        return *this;
    }


}

#endif //MESSENGER_BUFFERCOMPOSER_HPP
