#include "Context.hpp"

Context::Context(const std::string &configFile)
    : BaseContext(configFile)
    , mServer(nullptr)
{
    sslSetup();
    mPort = getPropertyTree().get("config.port", DEFAULT_PORT);
}

void Context::setServerPtr(ServerPtr server)
{
    mServer.swap(server);
}

const Context::ServerPtr& Context::getServer() const
{
    return mServer;
}

bool Context::isServerSet() const
{
    return (bool)mServer;
}

unsigned short Context::getPort() const
{
    return mPort;
}

void Context::sslSetup() {
    auto& ptree = getPropertyTree();
    std::string chain_file = ptree.get("config.ssl.cert" , DEFAULT_CERT_CHAIN_FILE);
    std::string key_file   = ptree.get("config.ssl.key"  ,        DEFAULT_KEY_FILE);
    std::string dh_file    = ptree.get("config.ssl.dh"   ,         DEFAULT_DH_FILE);

    auto file_format = ptree.get("config.ssl.key.<xmlattr>.asn1", false)
                       ? boost::asio::ssl::context::asn1
                       : boost::asio::ssl::context::pem;

    auto& sslContext = getSslContext();
    sslContext.use_certificate_chain_file(chain_file);
    sslContext.use_private_key_file(key_file, file_format);
    sslContext.use_tmp_dh_file(dh_file);
}
