#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/Logger/LogLevel.hpp"

inline void from_json(const nlohmann::json& j, LogLevel& l) { fromString(l, j.get< std::string >()); }