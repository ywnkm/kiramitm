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

    packet_builder::packet_builder(size_t init_len) : _size(0), _capacity(init_len), _position(0) {
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

    size_t packet_builder::size() const {
        return this->_size;
    }

    void packet_builder::put_string(std::string_view str) {
        auto size = str.size();
        if (this->_position + size > this->capacity()) {
            this->expand(this->capacity() * 2);
        }
        memcpy(this->_data + this->_position, str.data(), size);
        this->_position += size;
        this->_size += size;
    }

    void packet_builder::put_data(const void *data, size_t len) {
        if (this->_position + len > this->capacity()) {
            this->expand(this->capacity() * 2);
        }
        memcpy(this->_data + this->_position, data, len);
        this->_position += len;
        this->_size += len;
    }

    std::unique_ptr<void> packet_builder::build() const {
        // copy this.data to unique_ptr
        auto ptr = std::unique_ptr<void>(new char[this->_size]);
        if (ptr == nullptr) {
            spdlog::error("packet_builder::build: new failed");
            return nullptr;
        }
        memcpy(ptr.get(), this->_data, this->_size);
        return ptr;
    }

    packet_builder::~packet_builder() {
        if (this->_data != nullptr) {
            free(this->_data);
        }
    }

    void packet_builder::expand(size_t len) {
        if (len <= this->_capacity) { [[unlikely]]
            spdlog::warn("packet_builder::expand: len <= this->_capacity");
            return;
        }
        auto new_data = realloc(this->_data, len);
        if (new_data == nullptr) { [[unlikely]]
            spdlog::error("packet_builder::expand: realloc failed");
            return;
        }
        this->_data = new_data;
        this->_capacity = len;
    }
}

