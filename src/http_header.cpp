
#include "http_header.hpp"
#include <iostream>
#include <strings.h>

namespace krkr {

    http_headers::http_headers(const std::vector<std::pair<std::string, std::string>> &values) : _values(values) {}

    http_headers::http_headers() {
        this->_values = std::vector<std::pair<std::string, std::string>>();
    }

    http_headers::http_headers(const http_headers& other) = default;

    http_headers::http_headers(http_headers&& other) noexcept : _values(std::move(other._values)) {
    }

    http_headers http_headers::copy() const {
        auto values = this->_values;
        return http_headers{values};
    }

    void http_headers::add(const std::string &key, const std::string &value) {
        this->_values.emplace_back(key, value);
    }

    void http_headers::set(const std::string &key, const std::string &value) {
        auto it = std::ranges::find_if(this->_values, [&](const auto &item) {
            return strcasecmp(key.c_str(), item.first.c_str()) == 0;
        });
        if (it != this->_values.end()) {
            it->second = value;
        } else {
            add(key, value);
        }
    }

    std::optional<std::string> http_headers::get(const std::string& key) const {
        auto it = std::ranges::find_if(this->_values, [&](const auto &item) {
            return strcasecmp(key.c_str(), item.first.c_str()) == 0;
        });
        if (it != this->_values.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::string> http_headers::operator[](const std::string &key) const {
        return this->get(key);
    }



    bool http_headers::remove(const std::string &key) {
        auto newEnd = std::ranges::remove_if(_values, [&](const auto &element) {
            return element.first == key;
        }).begin();
        bool removed = (newEnd != _values.end());
        _values.erase(newEnd, _values.end());
        return removed;
    }

    std::ostream &operator<<(std::ostream &os, const http_headers &headers) {
        os << "Headers: " << headers.size() << "\n";
        for (const auto &[fst, snd] : headers) {
            os << fst << ": " << snd << "\r\n";
        }

        return os;
    }

    http_headers parseHttpHeaders(std::string_view str) {
        std::vector<std::pair<std::string, std::string>> values;

        size_t pos = 0;
        while (pos != std::string_view::npos) {
            auto end = str.find("\r\n", pos);
            if (end == std::string_view::npos) {
                break;
            }
            auto line = str.substr(pos, end - pos);
            auto colonPos = line.find(':');
            if (colonPos != std::string_view::npos) {
                values.emplace_back(std::string(line.substr(0, colonPos)),std::string(line.substr(colonPos + 2)));
            }
            pos = end + 2;
        }
        return http_headers{values};
    }

}
