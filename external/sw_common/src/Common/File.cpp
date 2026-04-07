#include "Common/File.hpp"

std::ofstream writeFile(const std::filesystem::path& filePath) {
    std::ofstream file(filePath);

    if (!file.is_open())
        throw std::runtime_error("Could not open write file " + filePath.string());

    return file;
}

std::ifstream readFile(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open())
        throw std::runtime_error("Could not open read file " + filePath.string());

    return file;
}

std::string readFileAsText(const std::filesystem::path& filePath) {
    std::ifstream file = readFile(filePath);

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();
    return buffer.str();
}