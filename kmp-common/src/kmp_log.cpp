
#include <kmp_log.hpp>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace kmp {

    static bool loggerInitialized = false;

    static std::shared_ptr<spdlog::logger> initLogger() {
        auto logger = spdlog::stdout_color_mt("kmp");
        logger->set_level(spdlog::level::trace);
        return logger;
    }

    static std::shared_ptr<spdlog::logger> instance = initLogger();

    spdlog::logger *Log::getLoggerInstance() {
        return instance.get();
    }
} // namespace kmp
