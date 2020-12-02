#ifndef ASIOAPPLICATION_ERROR_HPP
#define ASIOAPPLICATION_ERROR_HPP

#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>

#include "ErrorCategory.hpp"
#include "ErrorCode.hpp"


namespace Commons::System {

    class Error
        : public std::runtime_error
    {
    public:
        using error_code_t = ErrorCode::error_code_t;

    public:
        // Creates error with OK ErrorCode from Generic Category
        Error();

        explicit Error(const ErrorCode&);
        Error(const ErrorCode&, const std::string& message);
        Error(error_code_t code, const ErrorCategory& category);
        Error(error_code_t code, const ErrorCategory& category, const std::string& message);
        Error(const Error&) noexcept = default;

        [[nodiscard]] const std::string& getMessage() const;
        [[nodiscard]] const ErrorCode& getErrorCode() const;

        [[nodiscard]] const char* what() const override;

    private:
        ErrorCode mErrorCode;
        std::string mMessage;
    };

}

#endif //ASIOAPPLICATION_ERROR_HPP
