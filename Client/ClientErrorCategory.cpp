#include "ClientErrorCategory.hpp"

std::unique_ptr<ClientErrorCategory> ClientErrorCategory::_instance{};

const char* ClientErrorCategory::getName() const
{
    return "Client Error";
}

const char* ClientErrorCategory::getMessage(error_code_t code) const
{
    for (const auto& entry : errorTable)
        if (entry.first == code)
            return entry.second;

    return nullptr;
}

const ErrorCategory& ClientCategory()
{
    if (!ClientErrorCategory::_instance)
        ClientErrorCategory::_instance.reset(new ClientErrorCategory());

    return *ClientErrorCategory::_instance;
}
