#pragma once
#include <filesystem>
#include "Common/JSON/File.hpp"

extern std::filesystem::path ASSETS_FILE(const std::string& x);

class JSONReader {
public:
    static json readJSON(const std::string& fileName) { return readJSONFile(ASSETS_FILE(fileName)); }
};