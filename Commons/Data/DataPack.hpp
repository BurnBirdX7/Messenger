#ifndef MESSENGER_DATAPACK_HPP
#define MESSENGER_DATAPACK_HPP

#include <cassert>
#include <vector>

#include "IReceivable.hpp"
#include "ISendable.hpp"
#include "Types.hpp"

namespace Commons::Data {

    template <class Type>
    class DataPack
        : public IReceivable
        , public ISendable
    {
    public:
        // Just constructs
        DataPack() = default;

        // Reserves space for *count* elements
        explicit DataPack(size_t count);

        void add(const Type& message);
        const Type& at(size_t index) const;
        [[nodiscard]] size_t count() const;

        // IReceivable
        size_t fillFromBuffer(const ConstBuffer& buffer) override;

        // ISendable
        [[nodiscard]] ConstBufferVector getConstDataSequence() const override;
        [[nodiscard]] size_t bytes() const override;

        const std::vector<Type>& getVector() const;

    private:
        std::vector<Type> mPack;

    };


    template <class Type>
    inline DataPack<Type>::DataPack(size_t count)
    {
        mPack.reserve(count);
    }

    template <class Type>
    inline ISendable::ConstBufferVector DataPack<Type>::getConstDataSequence() const
    {
        ConstBufferVector seq;
        size_t toReserve = mPack.size() * Type::BUFFERS_COUNT;
        seq.reserve(toReserve);
        BufferComposer composer(seq);

        for (const auto& msg : mPack)
            composer.add(msg.getConstDataSequence());

        assert(toReserve == seq.size());

        return seq;
    }

    template <class Type>
    inline size_t DataPack<Type>::bytes() const
    {
        size_t bytes = 0;

        for (const auto& msg : mPack)
            bytes += msg.bytes();

        return bytes;
    }

    template <class Type>
    inline size_t DataPack<Type>::fillFromBuffer(const ConstBuffer& ref_buffer)
    {
        ConstBuffer buffer(ref_buffer);
        size_t before_read = buffer.size();

        try {
            while (buffer.size() > 0) {
                mPack.emplace_back();                           // create a new message
                auto& last = --mPack.end();                     // get it
                size_t bytes = last->fillFromBuffer(buffer);    // fill it

                buffer += bytes;
            }
        }
        catch (const std::runtime_error& error) {
            auto& last = --mPack.end();
            mPack.erase(last);
        }

        size_t after_read = buffer.size();

        return before_read - after_read;
    }

    template <class Type>
    inline void DataPack<Type>::add(const Type& message)
    {
        mPack.push_back(message);
    }

    template <class Type>
    inline const Type& DataPack<Type>::at(size_t index) const
    {
        return mPack.at(index);
    }

    template <class Type>
    inline size_t DataPack<Type>::count() const
    {
        return mPack.size();
    }

    template <class Type>
    inline const std::vector<Type>& DataPack<Type>::getVector() const
    {
        return mPack;
    }

}

#endif //MESSENGER_DATAPACK_HPP
