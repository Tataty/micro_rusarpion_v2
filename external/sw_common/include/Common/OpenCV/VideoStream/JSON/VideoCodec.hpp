#pragma once
#include "Common/OpenCV/JSON.hpp"

#include "Common/OpenCV/VideoStream/VideoCodec.hpp"

inline void from_json(const json& j, VideoCodec& c) { fromString(c, j.get< std::string >()); }
