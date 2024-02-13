#include <iostream>
#include <asio.hpp>
#include "http/http_server.hpp"
#include <spdlog/spdlog.h>

using asio::ip::tcp;

asio::awaitable<void> async_main() {
    auto executor = co_await asio::this_coro::executor;


    auto httpsServer = krkr::https_server();
    auto https_ports = co_await httpsServer.start();

    asio::co_spawn(executor, krkr::httpForwardServer(), asio::detached);
}

int main() {

    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("645");
    try {
        asio::io_context context{1};
        asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { context.stop(); });

        asio::co_spawn(context, async_main(), asio::detached);
        context.run();

    } catch (std::exception &e) {
        spdlog::error("Main err: {}", e.what());
    }
}