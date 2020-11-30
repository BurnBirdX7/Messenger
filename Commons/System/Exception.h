#ifndef ASIOAPPLICATION_EXCEPTION_H
#define ASIOAPPLICATION_EXCEPTION_H

#include <cstdint>
#include <stdexcept>

namespace Commons::System {

    class Error
        : public std::runtime_error
    {
    public:
        using error_code_t = uint32_t;

        static constexpr const char*  DEFAULT_ERROR_MSG  = " [ No message ] ";
        static constexpr error_code_t DEFAULT_ERROR_CODE = static_cast<uint32_t>(-1);

    public:
        Error();
        explicit Error(error_code_t error_code, const char* what_arg);
        explicit Error(error_code_t error_code, const std::string& what_arg);
        Error(const Error&) noexcept = default;

        [[nodiscard]] error_code_t getErrorCode() const;

        bool operator==(const Error&) const;

    private:
        error_code_t mErrorCode;

    };

}

#endif //ASIOAPPLICATION_EXCEPTION_H
