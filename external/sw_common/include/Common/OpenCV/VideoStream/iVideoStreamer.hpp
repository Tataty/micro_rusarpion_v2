#pragma once
#include <opencv2/opencv.hpp>

#include "Common/Module/iActiveModule.hpp"
#include "Common/OpenCV/VideoStream/VideoCodec.hpp"

class iVideoStreamer : public iActiveModule {
public:
    // TODO: title only in rtsp and other & make bigger universal streamer
    struct Config {
        std::string title;
        cv::Size    resolution;
        int         fps;
        /* TODO: use HostAddress */
        std::string ip   = "0.0.0.0";
        uint16_t    port = 8554;
        size_t      bitrate;
        VideoCodec  videoCodec;

        auto operator<=>(const Config&) const = default;
    };

public:
    iVideoStreamer(const std::shared_ptr< iModuleLogger >& logger) : iActiveModule(logger) {}

    virtual void pushFrame(const cv::Mat& frame) = 0;
};