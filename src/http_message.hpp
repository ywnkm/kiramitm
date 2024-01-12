#pragma once

#include <type_traits>
#include <concepts>
#include <map>
#include "http_header.hpp"

namespace krkr {

    template<typename T>
    concept http_message = requires(T t) {
        { t.headers() } -> std::convertible_to<http_headers>;
        t.setBody(nullptr);
        { t.getBody() } -> std::convertible_to<char *>;
    };

    namespace detail {
        class base_http_message {
            base_http_message(const char *data, size_t len) {}
        };
    }

    class http_request : public detail::base_http_message {

    };

    class http_response : public detail::base_http_message {

    };


}