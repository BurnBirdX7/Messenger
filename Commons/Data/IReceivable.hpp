#ifndef MESSENGER_IRECEIVABLE_HPP
#define MESSENGER_IRECEIVABLE_HPP

#include <vector>

namespace Commons::Data {

    class IReceivable
    {
    public:

        using MutableBuffer = boost::asio::mutable_buffer;
        using MutableBufferVector = std::vector<MutableBuffer>;

        virtual MutableBufferVector getMutableDataSequence() = 0;

    };



}

#endif //MESSENGER_IRECEIVABLE_HPP
