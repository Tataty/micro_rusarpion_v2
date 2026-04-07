#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/Type/Angle.hpp"

inline void from_json(const json& j, Angle& a) { a.setDegrees(j.get< double >()); }
inline void from_json(const json& j, Angle2& a2) {
    j.at("yaw").get_to(a2.yaw);
    j.at("pitch").get_to(a2.pitch);
}
