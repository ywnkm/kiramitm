#pragma once
#include <bit>
#include <memory>
#include <string_view>
#include <type_traits>
#include <sys/types.h>

namespace krkr {

    class packet_builder {
    public:
        explicit packet_builder(size_t init_len = 1024);

        [[nodiscard]]
        size_t capacity() const;

        [[nodiscard]]
        size_t position() const;

        [[nodiscard]]
        size_t size() const;

        void put_string(std::string_view str);

        void put_data(const void *data, size_t len);

        [[nodiscard]]
        std::unique_ptr<char[]> build() const;

        ~packet_builder();

        template<typename Integer>
        requires std::is_integral_v<Integer>
        void put_integer(Integer integer,const std::endian endian = std::endian::native) {
            constexpr auto size = sizeof(Integer);
            if (this->_position + size > this->capacity()) {
                this->expand(this->capacity() * 2);
            }
            if constexpr (size == 1) {
                *static_cast<u_int8_t *>(this->_data + this->_position) = static_cast<u_int8_t>(integer);
            } else if constexpr (size == 2) {
                if (endian == std::endian::native) {
                    *static_cast<u_int16_t *>(this->_data + this->_position) = static_cast<u_int16_t>(integer);
                } else {
                    *static_cast<u_int16_t *>(this->_data + this->_position) = std::byteswap(integer);
                }
            } else if constexpr (size == 4) {
                if (endian == std::endian::native) {
                    *static_cast<u_int32_t *>(this->_data + this->_position) = static_cast<u_int32_t>(integer);
                } else {
                    *static_cast<u_int32_t *>(this->_data + this->_position) = std::byteswap(integer);
                }
            } else if constexpr (size == 8) {
                if (endian == std::endian::native) {
                    *static_cast<u_int64_t *>(this->_data + this->_position) = static_cast<u_int64_t>(integer);
                } else {
                    *static_cast<u_int64_t *>(this->_data + this->_position) = std::byteswap(integer);
                }
            } else {
                static_assert(size == 1 || size == 2 || size == 4 || size == 8, "size must be 1, 2, 4 or 8");
            }
            this->_position += size;
        }

    private:

        void expand(size_t len);

    private:
        void *_data;
        size_t _size;
        size_t _capacity;
        size_t _position;
    };

} // namespace krkr
