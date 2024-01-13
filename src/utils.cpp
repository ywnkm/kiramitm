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

    namespace strings {
        bool is_all_digist(std::string_view str) {
            return std::ranges::all_of(str, [](auto c) {
                return std::isdigit(c);
            });
        }

        bool contains(std::string_view str1, std::string_view str2, bool ignore_case) {
            if (ignore_case) {
                return (str1.size() >= str2.size() &&
                std::equal(str2.begin(), str2.end(), str1.begin(),
                           [](auto a, auto b) { return std::tolower(a) == std::tolower(b); }));
            } else {
                return str1.find(str2) != std::string_view::npos;
            }
        }

        std::vector<std::string_view> split(std::string_view str, std::string_view delimiter) {
            auto result = std::vector<std::string_view>{};
            size_t pos = 0;
            while (true) {
                auto next_pos = str.find(delimiter, pos);
                if (next_pos == std::string_view::npos) {
                    result.emplace_back(str.substr(pos));
                    break;
                } else {
                    result.emplace_back(str.substr(pos, next_pos - pos));
                    pos = next_pos + delimiter.size();
                }
            }
            return result;
        }
    }

}
