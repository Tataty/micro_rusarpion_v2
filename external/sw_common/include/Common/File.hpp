#pragma once

#include <filesystem>
#include <fstream>

std::ofstream writeFile(const std::filesystem::path& filePath);
std::ifstream readFile(const std::filesystem::path& filePath);
std::string   readFileAsText(const std::filesystem::path& filePath);