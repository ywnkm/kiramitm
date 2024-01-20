#include <iostream>
#include <asio.hpp>
#include "http/http_server.hpp"
#include <spdlog/spdlog.h>

using asio::ip::tcp;

asio::awaitable<int> foo() {
    co_return 10;
}

asio::awaitable<void> listener() {
    auto executor = co_await asio::this_coro::executor;
    auto acceptor = tcp::acceptor(executor, {asio::ip::tcp::v4(), 55555});
    while (true) {
        auto socket = co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(executor, []() -> asio::awaitable<void> {
            co_await foo();
        }, asio::detached);
    }
}

int main() {

    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("645");
    try {
        asio::io_context context{1};
        asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { context.stop(); });

        asio::co_spawn(context, krkr::httpForwardServer(), asio::detached);
        auto httpsServer = krkr::https_server(8443);
        asio::co_spawn(context, httpsServer.start(), asio::detached);

        context.run();

    } catch (std::exception &e) {
        spdlog::error("Main err: {}", e.what());
    }
}