#ifndef MESSENGER_CLIENT_CONTEXT_HPP
#define MESSENGER_CLIENT_CONTEXT_HPP

#include <string>
#include <boost/asio.hpp>
#include <System/BaseContext.hpp>



/*
 * Client-side Context class
 */
class Context
        : public Commons::System::BaseContext
{
public:
    using IoContext = boost::asio::io_context;

public:
    static constexpr const char* DEFAULT_CONFIG_FILE = "client_config.xml";
    static constexpr const char* DEFAULT_CA_FILE     = "ca.pem";
    static constexpr const char* DEFAULT_PORT        = "56666";

public:
    explicit Context(const std::string& configFile = DEFAULT_CONFIG_FILE);

    void run();

    [[nodiscard]] const std::string& getServerAddress() const;
    [[nodiscard]] const std::string& getServerPort() const;

private:
    std::string mServerAddress;
    std::string mServerPort;


};


#endif //MESSENGER_CLIENT_CONTEXT_HPP
