#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/Communication/HostAddress.hpp"

inline void from_json(const nlohmann::json& j, HostAddress& h) {
    j.at("ip").get_to(h.ip);
    j.at("port").get_to(h.port);
}