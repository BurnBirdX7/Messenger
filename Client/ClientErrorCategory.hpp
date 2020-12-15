#ifndef MESSENGER_CLIENTERRORCATEGORY_HPP
#define MESSENGER_CLIENTERRORCATEGORY_HPP

#include <System/Exception.hpp>

using Commons::System::Error;
using Commons::System::ErrorCode;
using Commons::System::ErrorCategory;

const ErrorCategory& ClientCategory();

class ClientErrorCategory
        : public ErrorCategory
{
public:
    using Self = ClientErrorCategory;

public:
    enum ErrorCode : error_code_t
    {
        OK = 0,
        UNKNOWN = 1,
        HANDLERS_ARENT_SET = 2,
        NOTIFICATION_H_ISNT_SET = 3,
        STATE_H_ISNT_SET = 4
    };

    static constexpr ErrorTable<5> errorTable = {
            ErrorEntry{OK,                      "OK"                             },
            ErrorEntry{UNKNOWN,                 "Unknown"                        },
            ErrorEntry{HANDLERS_ARENT_SET,      "Listeners Aren't Set"           },
            ErrorEntry{NOTIFICATION_H_ISNT_SET, "Notification Handler Isn't Set" },
            ErrorEntry{STATE_H_ISNT_SET,        "State Change Handler Isn't Set" },
    };

public:
    [[nodiscard]]
    const char* getName() const override;

    [[nodiscard]]
    const char* getMessage(error_code_t code) const override;

public:
    friend const ErrorCategory& ClientCategory();

private:
    ClientErrorCategory() = default;

    static std::unique_ptr<Self> _instance;

};


#endif //MESSENGER_CLIENTERRORCATEGORY_HPP
