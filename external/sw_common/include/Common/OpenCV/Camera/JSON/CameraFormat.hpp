#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/OpenCV/Camera/CameraFormat.hpp"

inline void from_json(const nlohmann::json& j, CameraFormat& f) { f = CameraFormat(j.get< std::string >()); }
inline void to_json(nlohmann::json& j, const CameraFormat& f) { j = f.toString(); }