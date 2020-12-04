#ifndef ASIOAPPLICATION_NETWORKCATEGORY_HPP
#define ASIOAPPLICATION_NETWORKCATEGORY_HPP

#include "../System/ErrorCategory.hpp"

#include <memory>
#include "../System/ErrorCode.hpp"
#include "../System/Error.hpp"


namespace Commons::Network {

    using Commons::System::ErrorCategory;
    using Commons::System::ErrorCode;
    using Commons::System::Error;

    const ErrorCategory& NetworkCategory();

    class NetworkErrorCategory
        : public ErrorCategory
    {
    private:
        using Self = NetworkErrorCategory;

    public:
        enum ErrorCode : error_code_t
        {
            OK = 0,
            CONNECTION_TROUBLE, // TODO: Specify
            SSL_LISTENERS,
            SSL_SD_RUN_CONN
        };

        static constexpr ErrorTable<4> errorTable = {
                ErrorEntry {OK,                 "OK"                                },
                ErrorEntry {CONNECTION_TROUBLE, "Connection Trouble"                },
                ErrorEntry {SSL_LISTENERS,      "SSL Listeners"                     },
                ErrorEntry {SSL_SD_RUN_CONN,    "SSL Shutdown of Running Connection"},
        };

    public:
        // Returns name of the category
        [[nodiscard]] const char* getName() const override
        {
            return "Network";
        }

        // Returns message associated with the code.
        // If the code doesn't belong to the category, returns nullptr
        [[nodiscard]] const char* getMessage(error_code_t code) const override
        {
            for (const auto& entry : errorTable)
                if (entry.first == code)
                    return entry.second;
            return nullptr;
        }

    public:
        friend const ErrorCategory& NetworkCategory()
        {
            if (!_instance) {
                std::unique_ptr<Self> unique(new Self());
                _instance.swap(unique);
            }
            return *_instance;
        }

    private:
        static std::unique_ptr<Self> _instance;

    };

}


#endif //ASIOAPPLICATION_NETWORKCATEGORY_HPP
