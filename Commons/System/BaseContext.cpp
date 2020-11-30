#include "BaseContext.hpp"

using namespace Commons::System;

namespace pt = boost::property_tree;



BaseContext::BaseContext(const std::string& configFile)
    : mIoContext()
    , mSslContext(SslContext::sslv23)
    , mProperties()
    , mIoThreadCount(DEFAULT_IO_THREAD_COUNT)
{
    pt::read_xml(configFile, mProperties);
    setSslOptions();

    auto threads = mProperties.get_optional<unsigned short>("config.io_threads");
    if (threads.has_value() && threads.value() > 0 && threads.value() <= MAXIMUM_IO_THREAD_COUNT)
        mIoThreadCount = threads.value();

}

BaseContext::IoContext& BaseContext::getIoContext()
{
    return mIoContext;
}

BaseContext::SslContext& BaseContext::getSslContext()
{
    return mSslContext;
}

unsigned short BaseContext::getIoThreadCount() const
{
    return mIoThreadCount;
}

BaseContext::PTree& BaseContext::getPropertyTree()
{
    return mProperties;
}

long BaseContext::sslOptionFromString(const std::string& str)
{
    for (const auto& pair : SSL_OPTIONS)
        if (str == pair.first)
            return pair.second;

    return 0;
}

void BaseContext::setSslOptions()
{
    long optFlag = 0;

    auto options = mProperties.get_child_optional("config.ssl.options");
    if (options.has_value()) {
        for (const auto &option : *options) {
            auto str = option.second.get_value_optional<std::string>();
            if (str.has_value())
                optFlag |= this->sslOptionFromString(str.value());
        }
    }
    mSslContext.set_options(optFlag);
}
