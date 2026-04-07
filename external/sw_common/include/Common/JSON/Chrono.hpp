#pragma once

#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace std::chrono {

void from_json(const json& j, duration< double >& ms) { ms = duration< double >(j.get< double >()); }
void to_json(json& j, const duration< double >& ms) { j = ms.count(); }

}