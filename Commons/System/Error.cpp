#include "Error.hpp"

using namespace Commons::System;

Error::Error()
    : std::runtime_error("")
    , mErrorCode()
    , mMessage()
{}

Error::Error(const ErrorCode& errorCode)
    : std::runtime_error(errorCode.getMessage())
    , mErrorCode(errorCode)
    , mMessage()

{}

Error::Error(const ErrorCode& errorCode, const std::string& message)
    : std::runtime_error((errorCode.getMessage() + (": " + message)).c_str())
    , mErrorCode(errorCode)
    , mMessage(message)
{
}


Error::Error(error_code_t code, const ErrorCategory& category)
    : std::runtime_error(category.getMessage(code))
    , mErrorCode(code, category)
    , mMessage()
{}

Error::Error(error_code_t code, const ErrorCategory& category, const std::string& message)
    : std::runtime_error((category.getMessage(code) + (": " + message)).c_str())
    , mErrorCode(code, category)
    , mMessage(message)
{}


const char *Error::what() const
{
    return exception::what();
}

const std::string &Error::getMessage() const {
    return mMessage;
}

const ErrorCode & Error::getErrorCode() const
{
    return mErrorCode;
}