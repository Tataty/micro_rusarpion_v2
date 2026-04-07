#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <optional>
#include <string>

inline std::optional< std::string > parseJSONOptionalString(const json& j, const std::string& nameVariable) {
    if (j.contains(nameVariable)) {
        std::string parseString = j.at(nameVariable).get< std::string >();

        if (parseString.empty())
            return std::nullopt;
        else
            return parseString;
    }

    return std::nullopt;
}