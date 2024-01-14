#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <string_view>


namespace krkr {

    class http_headers {
    public:
        explicit http_headers(const std::vector<std::pair<std::string, std::string>> &values);
        http_headers();
        http_headers(const http_headers &);
        http_headers &operator=(const http_headers &) = delete;

        http_headers &operator=(http_headers &&other) noexcept;

        http_headers(http_headers &&other) noexcept;

        [[nodiscard]]
        http_headers copy() const;

        void add(const std::string &key, const std::string &value);

        void set(const std::string &key, const std::string &value);

        [[nodiscard]]
        std::optional<std::string> get(const std::string &key) const;

        [[nodiscard]]
        std::optional<std::string> operator[](const std::string &key) const;

        bool remove(const std::string &key);

        [[nodiscard]]
        auto begin() const {
            return this->_values.begin();
        }

        [[nodiscard]]
        auto begin() {
            return this->_values.begin();
        }

        [[nodiscard]]
        auto end() const {
            return this->_values.end();
        }

        [[nodiscard]]
        auto end() {
            return this->_values.end();
        }

        [[nodiscard]]
        size_t size() const {
            return this->_values.size();
        }

    private:

        std::vector<std::pair<std::string, std::string>> _values;
    };

    std::ostream &operator<<(std::ostream &os, const http_headers &headers);

    http_headers parseHttpHeaders(std::string_view str);

}
