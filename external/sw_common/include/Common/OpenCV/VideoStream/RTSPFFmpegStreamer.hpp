#pragma once

#include <fmt/core.h>

#include "Common/OpenCV/Common.hpp"
#include "Common/OpenCV/VideoStream/PipeVideoStreamer.hpp"

// TODO: add hardware encoding support
class RTSPFFmpegStreamer : public PipeVideoStreamer {
public:
    struct Config {
        iVideoStreamer::Config       streamer;
        std::optional< std::string > alsaMicrophone;
    };

private:
    PipeVideoStreamer::Config buildFFmpegStreamerConfig(Config config) {

        PipeVideoStreamer::Config ffmpegStreamerConfig;
        ffmpegStreamerConfig.streamer = config.streamer;

        std::string alsaInputDevice = "", alsaFormat = "";

        if (config.alsaMicrophone.has_value()) {

            alsaInputDevice = fmt::format("-f alsa -channels 1 -i {}", config.alsaMicrophone.value());
            alsaFormat      = "-c:a libopus -b:a 64k -ac 1";
        }

        switch (config.streamer.videoCodec) {

        // TODO: implementation fluent-builder
        case VideoCodec::MJPEG:
            ffmpegStreamerConfig.pipeline = fmt::format(
                    "ffmpeg -nostdin -f rawvideo -pixel_format yuv420p -video_size {resolution} -framerate {fps} -i - {alsa_device} -fflags nobuffer -c:v mjpeg -q:v 5 -huffman 0 -force_duplicated_matrix 1 -muxdelay 0 -muxpreload 0 -b:v {bitrate} -maxrate {maxrate} -bufsize {bufsize} {alsa_format} -f rtsp {stream_link}",
                    fmt::arg("resolution", toString(config.streamer.resolution)),
                    fmt::arg("fps", config.streamer.fps),
                    fmt::arg("alsa_device", alsaInputDevice),
                    fmt::arg("bitrate", config.streamer.bitrate),
                    fmt::arg("maxrate", config.streamer.bitrate),
                    fmt::arg("bufsize", config.streamer.bitrate / 2),
                    fmt::arg("alsa_format", alsaFormat),
                    fmt::arg("stream_link", buildStreamLink(config)));
            break;

        case VideoCodec::H265:
            ffmpegStreamerConfig.pipeline = fmt::format(
                    "ffmpeg -nostdin -f rawvideo -pixel_format yuv420p -video_size {resolution} -framerate {fps} -i - {alsa_device} -fflags nobuffer -flags low_delay -c:v libx265 -preset veryfast -tune zerolatency -muxdelay 0 -muxpreload 0 -b:v {bitrate} -maxrate {maxrate} -bufsize {bufsize} {alsa_format} -f rtsp {stream_link}",
                    fmt::arg("resolution", toString(config.streamer.resolution)),
                    fmt::arg("fps", config.streamer.fps),
                    fmt::arg("alsa_device", alsaInputDevice),
                    fmt::arg("bitrate", config.streamer.bitrate),
                    fmt::arg("maxrate", config.streamer.bitrate),
                    fmt::arg("bufsize", config.streamer.bitrate / 2),
                    fmt::arg("alsa_format", alsaFormat),
                    fmt::arg("stream_link", buildStreamLink(config)));
            break;

        case VideoCodec::H264:
        default:
            ffmpegStreamerConfig.pipeline = fmt::format(
                    "ffmpeg -nostdin -f rawvideo -pixel_format yuv420p -video_size {resolution} -framerate {fps} -i - {alsa_device} -fflags nobuffer -flags low_delay -c:v libx264 -preset veryfast -tune zerolatency -muxdelay 0 -muxpreload 0 -b:v {bitrate} -maxrate {maxrate} -bufsize {bufsize} {alsa_format} -f rtsp {stream_link}",
                    fmt::arg("resolution", toString(config.streamer.resolution)),
                    fmt::arg("fps", config.streamer.fps),
                    fmt::arg("alsa_device", alsaInputDevice),
                    fmt::arg("bitrate", config.streamer.bitrate),
                    fmt::arg("maxrate", config.streamer.bitrate),
                    fmt::arg("bufsize", config.streamer.bitrate / 2),
                    fmt::arg("alsa_format", alsaFormat),
                    fmt::arg("stream_link", buildStreamLink(config)));
            break;
        }

        return ffmpegStreamerConfig;
    }

public:
    RTSPFFmpegStreamer(std::shared_ptr< iModuleLogger >& logger, Config config)
        : PipeVideoStreamer(logger, buildFFmpegStreamerConfig(config)) {}

private:
    std::string buildStreamLink(Config config) {
        return fmt::format("rtsp://{}:{}/{}", config.streamer.ip, config.streamer.port, config.streamer.title);
    }
};