#pragma once
#include "nlohmann/json.hpp"

#include "Common/OpenCV/VideoStream/iVideoStreamer.hpp"

void from_json(const nlohmann::json& j, iVideoStreamer::Config& s);