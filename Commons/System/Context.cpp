//
// Created by artem on 20.11.2020.
//

#include "Context.hpp"

Context::Context()
    : mIoContext()
    , mSslContext(boost::asio::ssl::context::sslv23) // TODO: make context configurable
{}

Context::Context(boost::asio::ssl::context&& sslContext)
    : mIoContext()
    , mSslContext(std::move(sslContext))
{

}
