#pragma once

#include "Common/OpenCV/VideoStream/JSON/iVideoStreamer.hpp"
#include "Common/OpenCV/VideoStream/RTSPGStreamerStreamer.hpp"

inline void from_json(const json& j, RTSPGStreamerStreamer::Config& r) { j.at("streamer").get_to(r.streamer); }