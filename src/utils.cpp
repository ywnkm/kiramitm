#include "utils.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>

namespace krkr {

    asio::awaitable<void> delay(u_int64_t ms) {
        auto executor = co_await asio::this_coro::executor;
        auto timer = asio::steady_timer(executor, std::chrono::milliseconds(ms));
        co_await timer.async_wait(asio::use_awaitable);
    }

    void print_hex(const char *data, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << std::endl;
    }
}