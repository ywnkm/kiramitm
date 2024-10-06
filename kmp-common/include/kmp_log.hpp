#pragma once

#include <spdlog/spdlog.h>

#include "common_types.hpp"

namespace kmp {

    class Log : public AllStatic {
    public:

        static spdlog::logger *getLoggerInstance();

        template<typename ...Args>
        static void t(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            getLoggerInstance()->trace(fmt, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void d(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            getLoggerInstance()->debug(fmt, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void i(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            getLoggerInstance()->info(fmt, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void w(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            getLoggerInstance()->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void e(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            getLoggerInstance()->error(fmt, std::forward<Args>(args)...);
        }

    };

} // namespace kmp
