#pragma once
#include <cstdint>

#include "common_types.hpp"

namespace kmp::http2::frame {

    enum class FrameType : uint32_t {
        DATA = 0,
        HEADERS = 1,
        PRIORITY = 2,
        RST_STREAM = 3,
        SETTINGS = 4,
        PUSH_PROMISE = 5,
        PING = 6,
        GOAWAY = 7,
        WINDOW_UPDATE = 8,
        CONTINUATION = 9,
    };

    constexpr uint32_t HEADER_LENGTH = 9;
    constexpr uint32_t DEFAULT_MAX_SIZE = 0x4000;
    constexpr uint32_t MAX_MAX_SIZE = 0xffffff;

    class BaseFrame : public NonCopy {
    public:
        explicit BaseFrame(FrameType frameType) : mFrameType(frameType) {}

        virtual ~BaseFrame() = default;

        [[nodiscard]]
        FrameType getFrameType() const {
            return mFrameType;
        }

    private:
        FrameType mFrameType;
    };

} // namespace kmp::http2::frame
