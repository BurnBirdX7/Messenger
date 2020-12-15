#include "Context.hpp"
#include "Client.hpp"

Context::Context(const std::string& configFile)
    : BaseContext(configFile)
{
    auto& propertyTree = getPropertyTree();
    mServerAddress = propertyTree.get<std::string>("config.server.ipv4");
    mServerPort = propertyTree.get("config.server.port", std::string(DEFAULT_PORT));

    std::string verify_file = propertyTree.get("config.ssl.ca_cert", DEFAULT_CA_FILE);

    getSslContext().load_verify_file(verify_file);
}

void Context::run()
{
}

const std::string& Context::getServerAddress() const
{
    return mServerAddress;
}

const std::string& Context::getServerPort() const
{
    return mServerPort;
}
