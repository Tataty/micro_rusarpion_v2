#pragma once
#include "Common/OpenCV/JSON.hpp"

#include "Common/OpenCV/Camera/CameraRequirement.hpp"
#include "Common/OpenCV/Camera/JSON/CameraFormat.hpp"

inline void from_json(const json& j, CameraRequirement& c) {
    j.at("format").get_to(c.format);
    j.at("resolution").get_to(c.resolution);
    j.at("fps").get_to(c.fps);
}
