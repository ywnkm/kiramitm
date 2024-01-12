#pragma once

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <memory>
#include <utility>

namespace krkr {

    asio::awaitable<void> httpForwardServer(
            uint16_t port = 9002,
            uint16_t httpProxyPort = 8080,
            const std::string& httpProxyHost = "127.0.0.1",
            uint16_t httpsProxyPort = 8443,
            const std::string& httpsProxyHost = "127.0.0.1"
    );

    namespace detail {
        template<typename T>
        concept http_stream = std::is_same_v<T, asio::ip::tcp::socket>
                              || std::is_same_v<T, asio::ssl::stream<asio::ip::tcp::socket>>;

        template<http_stream Socket>
        class base_http_session {
        public:
            explicit base_http_session(std::shared_ptr<Socket> socket): _socket(socket) {

            }

            base_http_session(const base_http_session &) = delete;
            base_http_session &operator=(const base_http_session &) = delete;

            base_http_session(base_http_session &&other) noexcept : _socket(std::move((other._socket))) {
                other._socket = nullptr;
            }

            asio::awaitable<size_t> read(
                auto &&buffers
            ) {
                co_return co_await this->_socket->async_read_some(buffers, asio::use_awaitable);
            }

            asio::awaitable<size_t> write(
                auto &&buffers
            ) {
                co_return co_await this->_socket->async_write_some(buffers, asio::use_awaitable);
            }

        protected:
            std::shared_ptr<Socket> _socket;
        };
    }

    class http_session : public detail::base_http_session<asio::ip::tcp::socket > {

        asio::awaitable<void> foo() {


            auto arr = std::array<char, 30>{};
            auto buffers = asio::buffer(arr);
            auto b = co_await (*_socket).async_read_some(buffers, asio::use_awaitable);
            auto so = asio::ip::tcp::socket(co_await asio::this_coro::executor);
            auto a = this->read(buffers);
            co_await so.async_read_some(buffers, asio::use_awaitable);
        }

    };

    class https_session {
    public:
        https_session(const std::shared_ptr<asio::ip::tcp::socket>& socket, asio::ssl::context &context) : _socket(std::move(*socket), context) {}

        asio::awaitable<void> start();

    private:
        asio::awaitable<void> hand_shake();

    private:
        asio::ssl::stream<asio::ip::tcp::socket> _socket;
    };

    class https_server {

    public:
        explicit https_server(uint16_t port);

        https_server(const https_server &) = delete;
        https_server &operator=(const https_server &) = delete;

        asio::awaitable<void> start();

    private:
        asio::ssl::context _sslContext;
        uint16_t  _port;
    };

}