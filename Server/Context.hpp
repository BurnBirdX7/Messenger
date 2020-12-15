#ifndef MESSENGER_SERVER_CONTEXT_HPP
#define MESSENGER_SERVER_CONTEXT_HPP

#include <string>
#include <memory>
#include <optional>

#include <System/BaseContext.hpp>

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

public:
    explicit Context(const std::string& configFile = DEFAULT_CONFIG_FILE);

    [[nodiscard]] unsigned short getPort() const;

private:
    void sslSetup();

private:
    unsigned short mPort;

};


#endif //MESSENGER_SERVER_CONTEXT_HPP
