#pragma once
#include "nlohmann/json.hpp"

#include "Common/Ballistic/BallisticTable.hpp"

void from_json(const nlohmann::json& j, BallisticTable& bt);