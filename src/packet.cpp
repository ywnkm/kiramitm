//
// Created by ywnkm on 2024/1/13.
//

#include "packet.hpp"

#include <cstdlib>
#include <spdlog/spdlog.h>

namespace krkr {

    namespace detail {
        struct free_delete {
            void operator()(void *ptr) const {
                free(ptr);
            }
        };
    }

    packet_builder::packet_builder(size_t init_len) : _data(init_len) {
    }

    size_t packet_builder::capacity() const {
        return this->_data.capacity();
    }

    size_t packet_builder::size() const {
        return this->_data.size();
    }

    void packet_builder::put_string(std::string_view str) {
        put_data(str.data(), str.length());
    }

    void packet_builder::put_data(const void *data, size_t len) {
        auto *ptr = static_cast<const uint8_t *>(data);
        this->_data.insert(this->_data.end(), ptr, ptr + len);
    }

    packet_builder::~packet_builder() = default;

}

