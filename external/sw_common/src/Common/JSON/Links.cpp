#include "Common/JSON/Links.hpp"

#include "Common/File.hpp"
#include "Common/Text.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>

struct LinkFromTo {
    std::string from;
    std::string to;
};

void from_json(const json& j, LinkFromTo& l) {
    j.at("from").get_to(l.from);
    j.at("to").get_to(l.to);
}

std::string getLinkView(const std::string& s) { return "{" + s + "}"; }

json getJSONWithLinks(const json& jsonData) {

    std::string jsonText = jsonData.dump(4);

    std::vector< LinkFromTo > links = jsonData.at("links");

    bool wasReplace = true;
    while (wasReplace) {
        wasReplace = false;
        for (auto& link : links) {
            if (replaceAll(jsonText, getLinkView(link.from), link.to))
                wasReplace = true;
        }
    }

    return json::parse(jsonText);
}

json readJSONFileWithLinks(const std::filesystem::path& filePath) {

    std::string fileText = readFileAsText(filePath);
    json        jsonData = json::parse(fileText);

    std::vector< LinkFromTo > links = jsonData.at("links");

    bool wasReplace = true;
    while (wasReplace) {
        wasReplace = false;
        for (auto& link : links) {
            if (replaceAll(fileText, getLinkView(link.from), link.to))
                wasReplace = true;
        }
    }

    return json::parse(fileText);
}