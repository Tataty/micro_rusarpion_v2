#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <filesystem>

json readJSONFile(const std::filesystem::path& filePath);
void writeJSONToFile(const std::filesystem::path& filePath, const json& jsonData);
