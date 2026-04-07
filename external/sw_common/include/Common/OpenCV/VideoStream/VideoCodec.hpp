#pragma once

enum class VideoCodec {
    MJPEG,
    H264,
    H265,
};

inline void fromString(VideoCodec& videoCodec, const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);

    if (upperStr == "MJPEG") {
        videoCodec = VideoCodec::MJPEG;
    } else if (upperStr == "H264") {
        videoCodec = VideoCodec::H264;
    } else if (upperStr == "H265") {
        videoCodec = VideoCodec::H265;
    }
}
