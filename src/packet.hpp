#pragma once
#include <bit>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>
#include <sys/types.h>

namespace krkr {

    class packet_builder {
    public:
        explicit packet_builder(size_t init_len = 1024);

        [[nodiscard]]
        size_t capacity() const;

        [[nodiscard]]
        size_t size() const;

        void put_string(std::string_view str);

        void put_data(const void *data, size_t len);

        ~packet_builder();

        template<typename Integer>
        requires std::is_integral_v<Integer>
        void put_integer(Integer integer, const std::endian endian = std::endian::native) {
            constexpr auto size = sizeof(Integer);
            if constexpr (size == 1 || size == 2 || size == 4 || size == 8) {
                if (endian == std::endian::native) {
                    const uint8_t *ptr = reinterpret_cast<uint8_t *>(&integer);
                    put_data(ptr, size);
                } else {
                    const uint8_t *ptr = reinterpret_cast<uint8_t *>(&std::byteswap(integer));
                    put_data(ptr, size);
                }
            } else {
                static_assert(size == 1 || size == 2 || size == 4 || size == 8, "size must be 1, 2, 4 or 8");
            }
        }

        template<typename Float>
        requires std::is_floating_point_v<Float>
        void put_float(Float value, const std::endian endian = std::endian::native) {
            constexpr auto size = sizeof(Float);
            if constexpr (size == 4 || size == 8) {
                if (endian == std::endian::native) {
                    const uint8_t *ptr = reinterpret_cast<uint8_t *>(&value);
                    put_data(ptr, size);
                } else {
                    const uint8_t *ptr = reinterpret_cast<uint8_t *>(&std::bit_cast<Float>(std::byteswap(value)));
                    put_data(ptr, size);
                }
            } else {
                static_assert(size == 4 || size == 8, "size must be 4 or 8");
            }
        }

        std::vector<uint8_t> &data() {
            return this->_data;
        }

        [[nodiscard]]
        const std::vector<uint8_t> &data() const {
            return this->_data;
        }

        std::vector<uint8_t> build() & {
            return this->_data;
        }

        std::vector<uint8_t> build() && {
            return std::move(this->_data);
        }

    private:
        std::vector<uint8_t> _data;
    };

} // namespace krkr
