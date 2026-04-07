#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>

json getJSONWithLinks(const json& jsonData);
json readJSONFileWithLinks(const std::filesystem::path& filePath);