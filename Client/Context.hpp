#ifndef ASIOAPPLICATION_CONTEXT_HPP
#define ASIOAPPLICATION_CONTEXT_HPP

#include <string>

#include <System/BaseContext.hpp>

/*
 * Client-side Context class
 */
class Context
        : public Commons::System::BaseContext
{
public:
    static constexpr const char* DEFAULT_CONFIG_FILE = "client_config.xml";
    static constexpr const char* DEFAULT_CA_FILE     = "ca.pem";
    static constexpr const char* DEFAULT_PORT        = "56666";

public:
    explicit Context(const std::string& configFile = DEFAULT_CONFIG_FILE);

    [[nodiscard]] const std::string& getServerAddress() const;
    [[nodiscard]] const std::string& getServerPort() const;

private:
    std::string mServerAddress;
    std::string mServerPort;

};


#endif //ASIOAPPLICATION_CONTEXT_HPP
