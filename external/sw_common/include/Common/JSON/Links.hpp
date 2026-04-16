#pragma once
#include "nlohmann/json.hpp"

nlohmann::json resolveLinkedJSON(const nlohmann::json& json);
nlohmann::json readLinkedJSONFile(const std::filesystem::path& filePath);