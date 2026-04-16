#pragma once
#include "Common/Text.hpp"

enum class VideoCodec {
    MJPEG,
    H264,
    H265,
};

inline void fromString(VideoCodec& videoCodec, const std::string& str) {
    std::string upperStr = toUpper(str);

    if (upperStr == "MJPEG") {
        videoCodec = VideoCodec::MJPEG;
    } else if (upperStr == "H264") {
        videoCodec = VideoCodec::H264;
    } else if (upperStr == "H265") {
        videoCodec = VideoCodec::H265;
    } else {
        throw std::runtime_error("VideoCodec not found: " + str);
    }
}
