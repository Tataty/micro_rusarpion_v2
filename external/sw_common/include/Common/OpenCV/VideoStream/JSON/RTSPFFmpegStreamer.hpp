#pragma once

#include "Common/OpenCV/VideoStream/JSON/iVideoStreamer.hpp"
#include "Common/OpenCV/VideoStream/RTSPFFmpegStreamer.hpp"

inline void from_json(const json& j, RTSPFFmpegStreamer::Config& r) {
    j.at("streamer").get_to(r.streamer);

    if (j.contains("alsaMicrophone"))
        r.alsaMicrophone = j.at("alsaMicrophone").get< std::string >();
}