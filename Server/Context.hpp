#ifndef ASIOAPPLICATION_CONTEXT_HPP
#define ASIOAPPLICATION_CONTEXT_HPP

#include <string>
#include <memory>
#include <optional>

#include <System/BaseContext.hpp>

class Server; // pre-definition of Server class

/*
 * Server-side Context class
 */
class Context
        : public BaseContext
{
public:
    static constexpr const char* DEFAULT_CONFIG_FILE     = "server_config.xml";
    static constexpr const char* DEFAULT_CERT_CHAIN_FILE =    "server_crt.pem";
    static constexpr const char* DEFAULT_KEY_FILE        =   "private_key.pem";
    static constexpr const char* DEFAULT_DH_FILE         =        "dh2048.pem";

    using ServerPtr = std::shared_ptr<Server>;

public:
    explicit Context(const std::string& configFile = DEFAULT_CONFIG_FILE);

    void setServerPtr(ServerPtr);
    [[nodiscard]] const ServerPtr& getServer() const;
    [[nodiscard]] bool isServerSet() const;

private:
    void sslSetup();

private:
    std::shared_ptr<Server> mServer;

};


#endif //ASIOAPPLICATION_CONTEXT_HPP
