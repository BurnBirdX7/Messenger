#ifndef ASIOAPPLICATION_GENERICCATEGORY_HPP
#define ASIOAPPLICATION_GENERICCATEGORY_HPP

#include "ErrorCategory.hpp"

namespace Commons::System {

    const ErrorCategory& GenericCategory();

    class GenericErrorCategory
        : public ErrorCategory
    {
    private:
        using Self = GenericErrorCategory;

    public:
        enum ErrorCode : error_code_t
        {
            OK = 0,
            UNKNOWN = 1,
            BAD_PATH
        };

        static constexpr ErrorTable<3> errorTable = {
                ErrorEntry{OK,       "OK"       },
                ErrorEntry{UNKNOWN,  "Unknown"  },
                ErrorEntry{BAD_PATH, "Bad path" },
        };

    public:
        [[nodiscard]]
        const char* getName() const override
        {
            return "Generic";
        }

        [[nodiscard]]
        const char* getMessage(error_code_t code) const override
        {
            for (const auto& entry : errorTable)
                if (entry.first == code)
                    return entry.second;

            return nullptr;
        }

    public:
        friend const ErrorCategory& GenericCategory()
        {
            if (!_instance) {
                std::unique_ptr<Self> unique(new Self());
                _instance.swap(unique);
            }

            return *_instance;
        }

    private:
        GenericErrorCategory() = default;

        static std::unique_ptr<Self> _instance;

    };

}


#endif //ASIOAPPLICATION_GENERICCATEGORY_HPP
