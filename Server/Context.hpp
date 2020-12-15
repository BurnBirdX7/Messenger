#ifndef MESSENGER_SERVER_CONTEXT_HPP
#define MESSENGER_SERVER_CONTEXT_HPP

#include <string>
#include <memory>
#include <optional>

#include <System/BaseContext.hpp>

class Server; // pre-definition of Server class

/*
 * Server-side Context class
 */
class Context
        : public Commons::System::BaseContext
{
public:
    static constexpr const char* DEFAULT_CONFIG_FILE     = "server_config.xml";
    static constexpr const char* DEFAULT_CERT_CHAIN_FILE =    "server_crt.pem";
    static constexpr const char* DEFAULT_KEY_FILE        =   "private_key.pem";
    static constexpr const char* DEFAULT_DH_FILE         =        "dh2048.pem";
    static constexpr unsigned short DEFAULT_PORT = 56666;

    using ServerPtr = std::shared_ptr<Server>;

    friend Server;

public:
    explicit Context(const std::string& configFile = DEFAULT_CONFIG_FILE);

    [[nodiscard]] const ServerPtr& getServer() const;
    [[nodiscard]] bool isServerSet() const;

    [[nodiscard]] unsigned short getPort() const;

private:
    void sslSetup();
    void setServerPtr(ServerPtr);

private:
    std::shared_ptr<Server> mServer;
    unsigned short mPort;

};


#endif //MESSENGER_SERVER_CONTEXT_HPP
