#include "Exception.h"

using namespace Commons::System;

Error::Error()
    : std::runtime_error(DEFAULT_ERROR_MSG)
    , mErrorCode(DEFAULT_ERROR_CODE)
{}

Error::Error(error_code_t ec, const char* what_arg)
    : std::runtime_error(what_arg)
    , mErrorCode(ec)
{}

Error::Error(error_code_t ec,  const std::string& what_arg)
    : std::runtime_error(what_arg)
    , mErrorCode(ec)
{}

Error::error_code_t Error::getErrorCode() const
{
    return mErrorCode;
}

bool Error::operator==(const Error& other) const
{
    return this->getErrorCode() == other.getErrorCode();
}