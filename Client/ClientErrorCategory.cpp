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

friend const ErrorCategory& ClientCategory()
{
    if (!_instance)
        _instance.reset(new ClientErrorCategory())

    return *_instance;
}
