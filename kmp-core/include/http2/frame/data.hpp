#pragma once

#include <optional>
#include <buffer_type.hpp>
#include "frame_type.hpp"


namespace kmp::http2::frame {

    template<TBuffer TData>
    class Data final : public BaseFrame {
    public:
        static constexpr uint32_t FLAG_PADDED = 0x08;
        static constexpr uint32_t FLAG_END_STREAM = 0x01;


    public:
        Data(uint32_t streamId, TData data, bool endStream = false, std::optional<uint8_t> paddingLength = std::nullopt)
            : BaseFrame(FrameType::DATA), mStreamId(streamId), mEndStream(endStream), mData(data), mPaddingLength(paddingLength) {}

        [[nodiscard]]
        bool isPadded() const {
             return mPaddingLength.has_value();
        }

        [[nodiscard]]
        uint32_t getStreamId() const {
            return mStreamId;
        }

        [[nodiscard]]
        bool isEndStream() const {
            return mEndStream;
        }

        [[nodiscard]]
        auto &&getData(this auto &&self) {
            return self.mData;
        }

    private:
        uint32_t mStreamId;
        bool mEndStream;
        TData mData;
        std::optional<uint8_t> mPaddingLength;

    };

} // namespace kmp::http2::frame
