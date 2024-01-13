
#include "http_message.hpp"

#include <spdlog/spdlog.h>

#include "utils.hpp"

namespace krkr {
    namespace detail {
        base_http_message::base_http_message()
            : _headers(), _data(nullptr), _http_method(), _http_version(),
        _http_path("/"), _content_length(0), _keep_alive(-1) {
        }

        base_http_message::base_http_message(base_http_message&& other) noexcept
            : _headers(std::move(other._headers)), _data(std::move(other._data)),
        _http_method(std::move(other._http_method)), _http_version(std::move(other._http_version)),
        _http_path(std::move(other._http_path)), _content_length(other._content_length), _keep_alive(other._keep_alive){

        }

        base_http_message::base_http_message(const http_headers& headers, std::shared_ptr<char[]> data,
            size_t data_len) : _headers(headers), _data(std::move(data)),
        _http_method(), _http_version(http::version::HTTP_1_1), _http_path("/"), _content_length(data_len), _keep_alive(-1) {
        }

        base_http_message::base_http_message(http_headers&& headers, std::shared_ptr<char[]> data, size_t data_len)
            : _headers(std::move(headers)), _data(std::move(data)),
        _http_method(), _http_version(http::version::HTTP_1_1), _http_path("/"), _content_length(0), _keep_alive(-1) {
        }

        std::string &base_http_message::http_path() {
            return this->_http_path;
        }

        std::string &base_http_message::http_method() {
            return this->_http_method;
        }

        std::string &base_http_message::http_version() {
            return this->_http_version;
        }

        http_headers &base_http_message::headers() {
            return this->_headers;
        }

        size_t base_http_message::content_length() const {
            // if (this->_content_length >= 0) return this->_content_length;
            // auto value = this->_headers["Content-Length"].value_or("0");
            // if (strings::is_all_digist(value)) {
            //     this->_content_length = std::stol(value);
            // } else {
            //     this->_content_length = 0;
            //     spdlog::debug("Invalid Content-Length: {}", value);
            // }
            return this->_content_length;
        }

        bool base_http_message::keep_alive() const {
            switch (this->_keep_alive) {
                case -1: {
                    auto value = this->_headers["Connection"].value_or("");
                    if (strcasecmp("keep-alive", value.c_str()) != 0) {
                        this->_keep_alive = 1;
                    } else {
                        this->_keep_alive = 0;
                    }
                    return this->_keep_alive;
                }
                case 0: return false;
                case 1: return true;
                default: return false;
            }
        }
    }
}
