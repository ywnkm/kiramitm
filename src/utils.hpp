#pragma once

#include <asio.hpp>

namespace krkr {

    asio::awaitable<void> delay(uint64_t ms);

    void print_hex(const char *data, size_t len);

    namespace strings {
        bool is_all_digist(std::string_view str);

        bool contains(std::string_view str1, std::string_view str2, bool ignore_case = false);

        std::vector<std::string_view> split(std::string_view str, std::string_view delimiter);
    }
}