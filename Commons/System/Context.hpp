#ifndef ASIOAPPLICATION_CONTEXT_HPP
#define ASIOAPPLICATION_CONTEXT_HPP

#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class Context
{
public:

    Context();
    Context(boost::asio::ssl::context&&);

    Context(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(const Context&) = delete;
    Context& operator=(Context&&) = delete;

protected:

    boost::asio::io_context mIoContext;
    boost::asio::ssl::context mSslContext;

};


#endif //ASIOAPPLICATION_CONTEXT_HPP
