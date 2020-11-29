#ifndef ASIOAPPLICATION_BASECONTEXT_HPP
#define ASIOAPPLICATION_BASECONTEXT_HPP

#include <utility>
#include <string>
#include <array>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


class BaseContext
{
public:
    using IoContext = boost::asio::io_context;
    using SslContext = boost::asio::ssl::context;

    using option = std::pair<const char*, const long>;
    constexpr static std::array<option, 9> SSL_OPTIONS {
        option{"default_workarounds", SslContext::default_workarounds },
        option{"no_compression",      SslContext::no_compression      },
        option{"no_sslv2",            SslContext::no_sslv2            },
        option{"no_sslv3",            SslContext::no_sslv3            },
        option{"no_tlsv1",            SslContext::no_tlsv1            },
        option{"no_tlsv1_1",          SslContext::no_tlsv1_1          },
        option{"no_tlsv1_2",          SslContext::no_tlsv1_2          },
        option{"no_tlsv1_3",          SslContext::no_tlsv1_3          },
        option{"single_dh_use",       SslContext::single_dh_use       }
    };

public:
    explicit BaseContext(const std::string& configFile);

    BaseContext(const BaseContext&) = delete;
    BaseContext(BaseContext&&) = delete;
    BaseContext& operator=(const BaseContext&) = delete;
    BaseContext& operator=(BaseContext&&) = delete;

public:
    IoContext& getIoContext();
    SslContext& getSslContext();

protected:
    using PTree = boost::property_tree::ptree;
    PTree& getPropertyTree();

private:
    static long sslOptionFromString(const std::string&);
    void setSslOptions();

    IoContext mIoContext;
    SslContext mSslContext;
    PTree mProperties;

};

#endif //ASIOAPPLICATION_BASECONTEXT_HPP
