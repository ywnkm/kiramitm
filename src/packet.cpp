//
// Created by ywnkm on 2024/1/13.
//

#include "packet.hpp"

#include <cstdlib>
#include <spdlog/spdlog.h>

namespace krkr {

    packet_builder::packet_builder(size_t init_len) : _capacity(init_len), _position(0) {
        this->_data = malloc(init_len);
        if (this->_data == nullptr) {
            spdlog::error("packet_builder ctor: malloc failed");
        }
    }

    size_t packet_builder::capacity() const {
        return this->_capacity;
    }

    size_t packet_builder::position() const {
        return this->_position;
    }

    void packet_builder::put_string(std::string_view str) {
        auto size = str.size();
        if (this->_position + size > this->capacity()) {
            this->expand(this->capacity() * 2);
        }
        memcpy(this->_data + this->_position, str.data(), size);
        this->_position += size;
    }

    void packet_builder::put_data(const void *data, size_t len) {
        if (this->_position + len > this->capacity()) {
            this->expand(this->capacity() * 2);
        }
        memcpy(this->_data + this->_position, data, len);
        this->_position += len;
    }

    void packet_builder::expand(size_t len) {
        if (len <= this->_capacity) {
            spdlog::warn("packet_builder::expand: len <= this->_capacity");
            return;
        }
        auto new_data = realloc(this->_data, len);
        if (new_data == nullptr) { [[unlikely]]
            spdlog::error("packet_builder::expand: realloc failed");
            return;
        }
        this->_data = new_data;
    }
}

