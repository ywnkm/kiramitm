#include "http_server.hpp"
#include <iostream>
#include "../utils.hpp"
#include "http_header.hpp"
#include "http_message.hpp"
#include <spdlog/spdlog.h>

using asio::ip::tcp;

namespace krkr {

    namespace detail {

        static constexpr const char *connect = "CONNECT";
        static constexpr bool startWithConnect(const char *data) {
            for (int i = 0; i < 7; i++) {
                if (data[i] != connect[i]) [[unlikely]] return false;
            }
            return true;

        }

        static constexpr const char *connectEstablished = "HTTP/1.1 200 Connection established\r\n\r\n";
    }

    asio::awaitable<void> httpForwardServer(
            uint16_t port,
            uint16_t httpProxyPort,
            const std::string &httpProxyHost,
            uint16_t httpsProxyPort,
            const std::string &httpsProxyHost
    ) {
        spdlog::info("azzz");
        auto executor = co_await asio::this_coro::executor;
        std::unique_ptr<tcp::acceptor> acceptor;
        try {
            acceptor = std::make_unique<tcp::acceptor>(tcp::acceptor(executor, {asio::ip::tcp::v4(), port}));
        } catch (std::exception &e) {
            spdlog::error("Create Forward Server Error");
            spdlog::error(e.what());
            co_return;
        }
        spdlog::info("Forward http running at {}", port);
        while (true) {
            auto socket = std::make_shared<tcp::socket>(co_await acceptor->async_accept(asio::use_awaitable));
            spdlog::debug("accept connection");
            asio::co_spawn(executor, [=]() -> asio::awaitable<void> {
                auto buffer = std::array<u_char, 4096>{};
                auto readLen = co_await socket->async_read_some(asio::buffer(buffer), asio::use_awaitable);
                auto proxySocket = std::make_shared<tcp::socket>(executor);
                if (readLen > 8 && detail::startWithConnect((char *)buffer.data())) { [[likely]]
                    co_await socket->async_write_some(asio::buffer(detail::connectEstablished,  strlen(detail::connectEstablished)), asio::use_awaitable);
                    auto endpoint = tcp::endpoint(asio::ip::address::from_string(httpsProxyHost), httpsProxyPort);
                    co_await proxySocket->async_connect(endpoint, asio::use_awaitable);
                } else {
                    auto endpoint = tcp::endpoint(asio::ip::address::from_string(httpProxyHost), httpProxyPort);
                    co_await proxySocket->async_connect(endpoint, asio::use_awaitable);
                    co_await proxySocket->async_write_some(asio::buffer(buffer, readLen), asio::use_awaitable);
                }

                asio::co_spawn(executor, [socket, proxySocket]() -> asio::awaitable<void> {
                    auto buffer = std::array<char, 1024> {};
                    while (true) {
                        auto len = co_await socket->async_read_some(asio::buffer(buffer), asio::use_awaitable);
                        co_await proxySocket->async_write_some(asio::buffer(buffer, len), asio::use_awaitable);
                    }
                }, asio::detached);

                asio::co_spawn(executor, [socket, proxySocket]() -> asio::awaitable<void> {
                    auto buffer = std::array<char, 1024> {};
                    while (true) {
                        auto len = co_await proxySocket->async_read_some(asio::buffer(buffer), asio::use_awaitable);
                        co_await socket->async_write_some(asio::buffer(buffer, len), asio::use_awaitable);
                    }
                }, asio::detached);

            }, asio::detached);
        }
    }

    https_server::https_server(uint16_t port) : _sslContext(asio::ssl::context::sslv23), _port(port) {
        this->_sslContext.set_options(
             asio::ssl::context::default_workarounds
             | asio::ssl::context::no_sslv2
             | asio::ssl::context::single_dh_use
        );
        this->_sslContext.use_certificate_chain_file("cert.pem");
        this->_sslContext.use_private_key_file("key.pem", asio::ssl::context::pem);
    }

    asio::awaitable<void> https_server::start() {
        auto executor = co_await asio::this_coro::executor;
        std::unique_ptr<tcp::acceptor> acceptor;
        try {
            acceptor = std::make_unique<tcp::acceptor>(tcp::acceptor(executor, {asio::ip::tcp::v4(), this->_port}));
        } catch (std::exception &e) {
            spdlog::error("Create Https Server Error");
            spdlog::error(e.what());
            co_return;
        }
        spdlog::info("Https http running at {}", acceptor->local_endpoint().port());
        while (true) {
            auto socket = co_await acceptor->async_accept(asio::use_awaitable);
            asio::co_spawn(executor, [this, socket = std::move(socket)]() mutable -> asio::awaitable<void> {
                auto session = https_session(std::move(socket), this->_sslContext);
                co_await session.start();
            }, asio::detached);
        }
    }

    asio::awaitable<void> https_session::start() {
        co_await this->hand_shake();
        co_await parse_http_from_socket<http_request>(this->_socket);
        // auto buffer = std::array<char, 4096>{};
        // auto len = co_await this->_socket.async_read_some(asio::buffer(buffer), asio::use_awaitable);
        // spdlog::info("Rec: {}", std::string(buffer.data(), len));
        // const char *data = "HTTP/1.1 200\r\n\r\n";
        // co_await this->_socket.async_write_some(asio::buffer(data, strlen(data)), asio::use_awaitable);
        // co_await this->_socket.async_shutdown(asio::use_awaitable);
    }

    asio::awaitable<void> https_session::hand_shake() {
        co_await this->_socket.async_handshake(asio::ssl::stream_base::server, asio::use_awaitable);
    }
}