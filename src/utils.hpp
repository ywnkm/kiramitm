#pragma once

#include <asio.hpp>

namespace krkr {

    asio::awaitable<void> delay(u_int64_t ms);

    void print_hex(const char *data, size_t len);
}