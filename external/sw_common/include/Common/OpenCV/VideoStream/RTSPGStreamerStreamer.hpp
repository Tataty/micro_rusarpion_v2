#pragma once

#include <fmt/core.h>
#include "Common/OpenCV/Common.hpp"
#include "Common/OpenCV/VideoStream/PipeVideoStreamer.hpp"

class RTSPGStreamerStreamer : public PipeVideoStreamer {
public:
    struct Config {
        iVideoStreamer::Config streamer;
    };

private:
    PipeVideoStreamer::Config buildGStreamerConfig(Config config) {
        PipeVideoStreamer::Config gstStreamerConfig;
        gstStreamerConfig.streamer = config.streamer;

        gstStreamerConfig.pipeline = fmt::format(
                "gst-launch-1.0 fdsrc do-timestamp=true ! videoparse format=i420 width={width} height={height} framerate={fps}/1 ! "
                "v4l2h264enc extra-controls='controls,video_bitrate={bitrate}' ! 'video/x-h264,level=(string)4' ! h264parse ! "
                "rtspclientsink location={stream_link}",
                fmt::arg("width", config.streamer.resolution.width),
                fmt::arg("height", config.streamer.resolution.height),
                fmt::arg("fps", config.streamer.fps),
                fmt::arg("bitrate", config.streamer.bitrate),
                fmt::arg("stream_link", buildStreamLink(config)));

        return gstStreamerConfig;
    }

public:
    RTSPGStreamerStreamer(std::shared_ptr< iModuleLogger >& logger, Config config)
        : PipeVideoStreamer(logger, buildGStreamerConfig(config)) {}

private:
    std::string buildStreamLink(Config config) {
        return fmt::format("rtsp://{}:{}/{}", config.streamer.ip, config.streamer.port, config.streamer.title);
    }
};