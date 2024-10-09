#pragma once

#include <buffer_type.hpp>
#include <vector>

#include "frame_type.hpp"
#include "../error_code.hpp"

namespace kmp::http2::frame {

    template<TBuffer TData = std::vector<uint8_t>>
    class GoAway final : public BaseFrame {
    public:
        GoAway(uint32_t lastStreamId, ErrorCode errorCode, TData additionalData)
            : BaseFrame(FrameType::GOAWAY), mLastStreamId(lastStreamId), mErrorCode(errorCode), mAdditionalData(additionalData) {}

        [[nodiscard]]
        uint32_t getLastStreamId() const {
            return mLastStreamId;
        }

        [[nodiscard]]
        ErrorCode getErrorCode() const {
            return mErrorCode;
        }

        [[nodiscard]]
        auto &&getAdditionalData(this auto &&self) {
            return self.mAdditionalData;
        }

    private:
        uint32_t mLastStreamId;
        ErrorCode mErrorCode;
        TData mAdditionalData;
    };
} // namespace kmp::http2::frame
