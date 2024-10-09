#pragma once

#include <concepts>

namespace kmp {

    class NonCopy {
    public:
        NonCopy() = default;
        NonCopy(const NonCopy &) = delete;
        NonCopy &operator=(const NonCopy &) = delete;
    };

    class AllStatic {
    public:
        AllStatic() = delete;
        ~AllStatic() = delete;
    };

} // namespace kmp
