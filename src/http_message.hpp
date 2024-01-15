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

    template<typename T>
    concept socket = requires(T t) {
        { t.async_read_some(std::declval<asio::mutable_buffer>(), asio::use_awaitable) } -> std::convertible_to<asio::awaitable<size_t>>;
        { t.async_write_some(std::declval<asio::mutable_buffer>(), asio::use_awaitable) } -> std::convertible_to<asio::awaitable<size_t>>;
    };

    class http_request : public detail::base_http_message {

    };

    class http_response : public detail::base_http_message {

    public:

    };

    template<typename HttpMessage, socket Socket>
    requires std::is_base_of_v<detail::base_http_message, HttpMessage>
    asio::awaitable<HttpMessage> parse_http_from_socket(Socket &socket) {
        auto buffer = std::make_unique<std::array<char, 4096>>();
        auto headers = http_headers{};
        auto len = co_await socket.async_read_some(asio::buffer(*buffer), asio::use_awaitable);
        if (len == 0) co_return HttpMessage{};
        int index = -1;
        auto *end = strstr(buffer->data(), "\r\n");
        if (end == nullptr) { [[unlikely]]
            spdlog::error("Invalid http request: {}", buffer->data());
            throw std::runtime_error("Invalid http request");
        }
        index = end - buffer->data();
        if (index <= 0) { [[unlikely]]
            spdlog::error("Invalid http request: {}", buffer->data());
            throw std::runtime_error("Invalid http request");
        }
        auto first_line = std::string_view(buffer->data(), index);
        auto splits = strings::split(first_line, " ");
        if (splits.size() != 3) { [[unlikely]]
            spdlog::error("Invalid http request: {}", buffer->data());
            throw std::runtime_error("Invalid http request");
        }
        auto method = std::string_view(splits[0]);
        auto path = std::string_view(splits[1]);
        auto version = std::string_view(splits[2]);
        if (version != http::version::HTTP_1_1) { [[unlikely]]
            spdlog::warn("Unsuppored http request: {}", buffer->data());
        }

        auto header_end = strstr(buffer->data(), "\r\n\r\n");
        auto body_bytes = packet_builder{};

        if (header_end == nullptr) { [[unlikely]] // headers length > buffer length
            // TODO
            auto header_remainng = packet_builder{};
            throw std::runtime_error("TODO");
        } else {
            auto headers_string = std::string_view(buffer->data() + index + 2, header_end - buffer->data() - index);
            headers = parseHttpHeaders(headers_string);
        }
        auto content_length_str = headers["Content-Length"].value_or("0");
        size_t content_length = 0;
        if (strings::is_all_digist(content_length_str)) { [[likely]]
            content_length = std::stol(content_length_str);
            if (content_length > 0) {
                auto body_len = len - (header_end - buffer->data()) - 4;
                if (body_len > 0) {
                    body_bytes.put_data(buffer->data() + (header_end - buffer->data()) + 4, body_len);
                }
                while (body_bytes.position() < content_length) {
                    len = co_await socket.async_read_some(asio::buffer(*buffer), asio::use_awaitable);
                    body_bytes.put_data(buffer->data(), len);
                }
            }
        }

        // TODO
        auto result = http_request(headers, std::shared_ptr<char[]>(body_bytes.build()), content_length);

        result.http_method() = method;
        result.http_path() = path;
        result.http_version() = version;
        // split by space

        co_return result;
    }


}
