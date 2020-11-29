#include "Context.hpp"


Context::Context(const std::string& configFile)
    : BaseContext(configFile)
{
    auto& ptree = getPropertyTree();
    mServerAddress = ptree.get<std::string>("config.server.ipv4");
    mServerPort = ptree.get("config.server.port", std::string(DEFAULT_PORT));

    std::string verify_file = ptree.get("config.ssl.ca_cert", DEFAULT_CA_FILE);

    getSslContext().load_verify_file(verify_file);
}

const std::string& Context::getServerAddress() const
{
    return mServerAddress;
}

const std::string& Context::getServerPort() const
{
    return mServerPort;
}