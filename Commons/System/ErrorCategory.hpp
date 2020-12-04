#ifndef ASIOAPPLICATION_ERRORCATEGORY_HPP
#define ASIOAPPLICATION_ERRORCATEGORY_HPP

#include <cstdint>
#include <array>
#include <utility>
#include <memory>

namespace Commons::System {

    class ErrorCategory
    {
    public:
        using error_code_t = uint32_t;

        using ErrorEntry = std::pair<error_code_t , const char*>;

        template <size_t N>
        using ErrorTable = std::array<ErrorEntry, N>;

    public:

        constexpr ErrorCategory() noexcept = default;
        ErrorCategory(const ErrorCategory&) = delete;

        // Returns name of the category
        [[nodiscard]] virtual const char* getName() const = 0;

        // Returns message associated with the code.
        // If the code doesn't belong to the category, returns nullptr
        [[nodiscard]] virtual const char* getMessage(error_code_t code) const = 0;

        // Returns OK error code
        [[nodiscard]] inline virtual error_code_t OK() const
        {
            return 0;
        }

        inline bool operator==(const ErrorCategory& other)
        {
            return this == &other;
        }

    };

}





#endif //ASIOAPPLICATION_ERRORCATEGORY_HPP
