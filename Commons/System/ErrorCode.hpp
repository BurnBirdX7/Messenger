#ifndef ASIOAPPLICATION_ERRORCODE_HPP
#define ASIOAPPLICATION_ERRORCODE_HPP

#include <cstdint>
#include <string>

#include "ErrorCategory.hpp"
#include "GenericCategory.hpp"


namespace Commons::System {

    class ErrorCode
    {
    public:
        using error_code_t = ErrorCategory::error_code_t;

    public:

        // Creates OK Error Code for generic category
        ErrorCode();

        // Creates error code for the specified category
        ErrorCode(error_code_t code, const ErrorCategory&);

    public:
        // Returns Message for the code
        [[nodiscard]] const char*          getMessage()  const;

        // Returns the code
        [[nodiscard]] error_code_t         getValue()    const;

        // Returns the category
        [[nodiscard]] const ErrorCategory& getCategory() const;

        // Returns false if value == OK
        [[nodiscard]] explicit operator bool () const;

    private:
        error_code_t mValue;
        const ErrorCategory& mCategory;

    };

}

#endif //ASIOAPPLICATION_ERRORCODE_HPP
