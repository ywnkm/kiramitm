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
        memset(this->_data, 0, init_len);
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

    std::unique_ptr<char[]> packet_builder::build() const {
        try {
            auto ptr = std::make_unique<char[]>(this->_size);
            memcpy(ptr.get(), this->_data, this->_size);
            return ptr;
        } catch (const std::bad_alloc &e) {
            spdlog::error("packet_builder::build: Allocation failed - {}", e.what());
            return nullptr;
        }
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
        if (new_data != this->_data && this->_data != nullptr) {
            free(this->_data);
        }
        this->_data = new_data;
        this->_capacity = len;
        // memset(this->_data + this->_capacity, 0, len - this->_capacity);
    }
}

