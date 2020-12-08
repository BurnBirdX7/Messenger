#ifndef MESSENGER_ITRIVIALLYRECEIVABLE_HPP
#define MESSENGER_ITRIVIALLYRECEIVABLE_HPP

#include "IReceivable.hpp"

namespace Commons::Data {

    class ITriviallyReceivable
        : public IReceivable
    {
    public:
        using MutableBufferVector = std::vector<MutableBuffer>;

        virtual MutableBufferVector getMutableDataSequence() = 0;

    };

}

#endif //MESSENGER_ITRIVIALLYRECEIVABLE_HPP
