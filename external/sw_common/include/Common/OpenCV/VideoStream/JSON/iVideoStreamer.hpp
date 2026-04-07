#pragma once
#include "Common/OpenCV/JSON.hpp"

#include "Common/OpenCV/VideoStream/iVideoStreamer.hpp"

inline void from_json(const json& j, iVideoStreamer::Config& s) {
    j.at("title").get_to(s.title);
    j.at("resolution").get_to(s.resolution);
    j.at("fps").get_to(s.fps);
    j.at("ip").get_to(s.ip);
    j.at("port").get_to(s.port);
    j.at("bitrate").get_to(s.bitrate);
    j.at("videoCodec").get_to(s.videoCodec);
}