
#include "http_header.hpp"
#include <iostream>

namespace krkr {

    http_headers::http_headers(std::vector<std::pair<std::string, std::string>> &values) : _values(values) {}

    http_headers::http_headers() {
        this->_values = std::vector<std::pair<std::string, std::string>>();
    }

    http_headers http_headers::copy() const {
        auto values = this->_values;
        return http_headers{values};
    }

    void http_headers::add(const std::string &key, const std::string &value) {
        this->_values.emplace_back(key, value);
    }

    void http_headers::set(const std::string &key, const std::string &value) {
        auto it = std::find_if(this->_values.begin(), this->_values.end(), [&](const auto &item) {
            return key == item.first;
        });
        if (it != this->_values.end()) {
            it->second = value;
        } else {
            add(key, value);
        }
    }

    bool http_headers::remove(const std::string &key) {
        auto newEnd = std::remove_if(_values.begin(), _values.end(), [&](const auto &element) {
             return element.first == key;
        });
        bool removed = (newEnd != _values.end());
        _values.erase(newEnd, _values.end());
        return removed;
    }

    std::ostream &operator<<(std::ostream &os, const http_headers &headers) {
        os << "Headers: " << headers.size() << "\n";
        for (const auto &item : headers) {
            os << item.first << ": " << item.second << "\r\n";
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