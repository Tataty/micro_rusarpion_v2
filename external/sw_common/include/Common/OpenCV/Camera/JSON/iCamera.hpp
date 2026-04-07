#pragma once
#include "Common/OpenCV/Camera/iCamera.hpp"
#include "Common/Type/JSON/Angle.hpp"

inline void from_json(const json& j, CameraParameters& c) { j.at("angleOfView").get_to(c.angleOfView); }
