#include "Common/File.hpp"
#include "Common/JSON/File.hpp"

json readJSONFile(const std::filesystem::path& filePath) {
    std::ifstream file = readFile(filePath);

    json jsonData;
    try {
        file >> jsonData;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }

    file.close();
    return jsonData;
}

void writeJSONToFile(const std::filesystem::path& filePath, const json& jsonData) {
    std::ofstream file = writeFile(filePath);
    file << jsonData.dump(4);
    file.close();
}