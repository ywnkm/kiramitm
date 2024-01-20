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

        [[nodiscard]]
        std::unique_ptr<char[]> build() &&;

        ~packet_builder();

        template<typename Integer>
        requires std::is_integral_v<Integer>
        void put_integer(Integer integer, const std::endian endian = std::endian::native) {
            constexpr auto size = sizeof(Integer);
            if (this->_position + size > this->capacity()) {
                this->expand(this->capacity() * 2);
            }
            if constexpr (size == 1 || size == 2 || size == 4 || size == 8) {
                if (endian == std::endian::native) {
                    *(Integer *)(void *)(this->_data + this->_position) = integer;
                } else {
                    *(Integer *)(void *)(this->_data + this->_position) = std::byteswap(integer);
                }
            } else {
                static_assert(size == 1 || size == 2 || size == 4 || size == 8, "size must be 1, 2, 4 or 8");
            }
            this->_position += size;
            this->_size += size;
        }

        template<typename Float>
        requires std::is_floating_point_v<Float>
        void put_float(Float value, const std::endian endian = std::endian::native) {
            constexpr auto size = sizeof(Float);
            if (this->_position + size > this->capacity()) {
                this->expand(this->capacity() * 2);
            }

            if constexpr (size == 4 || size == 8) {
                Float swappedValue = (endian == std::endian::native) ? value : std::bit_cast<Float>(std::byteswap(value));
                memcpy(this->_data + this->_position, &swappedValue, size);
            } else {
                static_assert(size == 4 || size == 8, "size must be 4 or 8");
            }

            this->_position += size;
            this->_size += size;
        }

    private:

        void expand(size_t len);

    private:
        char *_data;
        size_t _size;
        size_t _capacity;
        size_t _position;
    };

} // namespace krkr
