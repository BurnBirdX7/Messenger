#include "ErrorCode.hpp"

using namespace Commons::System;

ErrorCode::ErrorCode()
    : mCategory(GenericCategory())
    , mValue(GenericCategory::OK)
{}

ErrorCode::ErrorCode(error_code_t code, const ErrorCategory& category)
    : mCategory(category)
    , mValue(code)
{}

const char* ErrorCode::getMessage() const {
    return mCategory.getMessage(mValue);
}

ErrorCode::error_code_t ErrorCode::getValue() const
{
    return mValue;
}

const ErrorCategory& ErrorCode::getCategory() const
{
    return mCategory;
}

ErrorCode::operator bool() const {
    return mCategory.OK() != mValue;
}
