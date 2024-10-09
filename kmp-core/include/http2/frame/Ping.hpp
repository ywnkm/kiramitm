#pragma once
#include <algorithm>
#include <span>

#include "frame_type.hpp"

namespace kmp::http2::frame {

    class Ping final : public BaseFrame {
    public:
        static constexpr uint32_t FLAG_ACK = 0x01;
        static constexpr uint32_t PAYLOAD_LENGTH = 8;

    public:
        Ping(bool ack, std::span<uint8_t, PAYLOAD_LENGTH> payload)
            : BaseFrame(FrameType::PING), mAck(ack) {
            std::copy_n(payload.data(), PAYLOAD_LENGTH, mPayload);
        }

        [[nodiscard]]
        bool isAck() const {
            return mAck;
        }

        [[nodiscard]]
        const uint8_t (&getPayload() const)[PAYLOAD_LENGTH] {
            return mPayload;
        }

    private:
        bool mAck;
        uint8_t mPayload[PAYLOAD_LENGTH]{};

    };
} // namespace kmp::http2::frame
