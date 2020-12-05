#ifndef MESSENGER_ISENDABLE_HPP
#define MESSENGER_ISENDABLE_HPP

#include <vector>
#include <boost/asio.hpp>

namespace Commons::Data {

    class ISendable
    {
    public:
        using ConstBuffer = boost::asio::const_buffer;
        using ConstBufferVector = std::vector<ConstBuffer>;

        virtual ConstBufferVector getConstDataSequence() const = 0;

    };

}

#endif //MESSENGER_ISENDABLE_HPP
