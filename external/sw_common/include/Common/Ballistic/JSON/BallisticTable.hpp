#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/Ballistic/BallisticTable.hpp"
#include "Common/Type/JSON/Angle.hpp"

static inline std::vector< double > angleToDegree(const std::vector< Angle >& angles) {
    std::vector< double > anglesInDegrees;

    for (const auto& angle : angles) {
        anglesInDegrees.push_back(angle.getDegrees());
    }

    return anglesInDegrees;
}

inline void from_json(const nlohmann::json& j, BallisticTable& bt) {

    std::vector< double > distances = j.at("distances").get< std::vector< double > >();
    std::vector< Angle >  angles    = j.at("angles").get< std::vector< Angle > >();
    std::vector< double > times     = j.at("times").get< std::vector< double > >();

    bt = BallisticTable(BallisticTable::Tables{
            .distances       = distances,
            .anglesInDegrees = angleToDegree(angles),
            .times           = times,
    });
}