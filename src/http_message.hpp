#pragma once

#include <type_traits>
#include <memory>
#include <asio.hpp>
#include <asio/ssl/stream.hpp>
#include <spdlog/spdlog.h>
#include "http_header.hpp"
#include "http_constance.hpp"
#include "utils.hpp"
#include "packet.hpp"

namespace krkr {

    namespace detail {
        class base_http_message {
        public:

            base_http_message();

            base_http_message(base_http_message &&other) noexcept;

            base_http_message(const http_headers &headers, std::shared_ptr<char []> data, size_t data_len);
            base_http_message(http_headers &&headers, std::shared_ptr<char []> data, size_t data_len);

            std::string &http_path();
            std::string &http_method();
            std::string &http_version();

            http_headers &headers();

            size_t content_length() const;

            bool keep_alive() const;

        private:
            http_headers _headers;
            std::shared_ptr<char []> _data;
            std::string _http_method;
            std::string _http_version;
            std::string _http_path;
            size_t _content_length;
            mutable int8_t _keep_alive = -1;
        };
    }

    class http_request : public detail::base_http_message {

    };

    class http_response : public detail::base_http_message {

    };

    template<typename HttpMessage, typename Socket>
    requires std::is_base_of_v<detail::base_http_message, HttpMessage>
    && (std::is_same_v<Socket, asio::ip::tcp::socket>
        || std::is_same_v<Socket, asio::ssl::stream<asio::ip::tcp::socket>>)
    asio::awaitable<HttpMessage> parse_http_from_socket(Socket &socket) {
        auto buffer = std::array<char, 4096>{};

        bool parse_headers = true;
        auto headers = http_headers{};
        auto len = co_await socket.async_read_some(asio::buffer(buffer), asio::use_awaitable);
        if (len == 0) co_return HttpMessage{};
        int index = -1;
        auto *end = strstr(buffer.data(), "\r\n");
        if (end == nullptr) { [[unlikely]]
            spdlog::error("Invalid http request: {}", buffer.data());
            throw std::runtime_error("Invalid http request");
        }
        index = end - buffer.data();
        if (index <= 0) { [[unlikely]]
            spdlog::error("Invalid http request: {}", buffer.data());
            throw std::runtime_error("Invalid http request");
        }
        auto first_line = std::string_view(buffer.data(), index);
        auto splits = strings::split(first_line, " ");
        if (splits.size() != 3) { [[unlikely]]
            spdlog::error("Invalid http request: {}", buffer.data());
            throw std::runtime_error("Invalid http request");
        }
        auto method = std::string_view(splits[0]);
        auto path = std::string_view(splits[1]);
        auto version = std::string_view(splits[2]);
        if (version != http::version::HTTP_1_1) { [[unlikely]]
            spdlog::warn("Unsuppored http request: {}", buffer.data());
        }

        auto header_end = strstr(buffer.data(), "\r\n\r\n");

        if (header_end == nullptr) { [[unlikely]] // headers too long
            // TODO
        } else {
            auto headers_string = std::string_view(buffer.data() + index + 2, header_end - buffer.data() - index - 2);
            while (true) {
                auto header_line = std::string_view(headers_string.data(), headers_string.find("\r\n"));
                auto header_splits = strings::split(header_line, ": ");
                if (header_splits.size() < 2) { [[unlikely]]
                    spdlog::error("Invalid http header: {}", header_line);
                    throw std::runtime_error("Invalid http header");
                }
                headers.add(std::string(header_splits[0]), std::string(header_splits[1]));
                headers_string = headers_string.substr(header_line.size() + 2);
                if (headers_string.empty()) {
                    break;
                }
            }
        }
        // TODO
        auto result = HttpMessage{headers, nullptr, 0};

        result.http_method() = method;
        result.http_path() = path;
        result.http_version() = version;
        // split by space

        co_return result;
    }


}
