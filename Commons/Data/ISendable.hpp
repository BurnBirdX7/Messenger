#ifndef MESSENGER_ISENDABLE_HPP
#define MESSENGER_ISENDABLE_HPP

#include <vector>

#include "Types.hpp"

namespace Commons::Data {

    class ISendable
    {
    public:
        using ConstBufferVector = std::vector<ConstBuffer>;

        virtual ConstBufferVector getConstDataSequence() const = 0;
        virtual size_t bytes() const = 0;

    };

}

#endif //MESSENGER_ISENDABLE_HPP