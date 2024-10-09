#pragma once

#include "common_types.hpp"

namespace kmp {

    template<typename T>
    concept TBuffer = requires(T t) {
        { t.data() } -> std::convertible_to<uint8_t *>;
        { t.size() } -> std::convertible_to<std::size_t>;
    };

} // namespace kmp
