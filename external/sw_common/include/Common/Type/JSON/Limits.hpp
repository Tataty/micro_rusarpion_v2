#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/Type/Limits.hpp"

template < typename T > void from_json(const nlohmann::json& j, Limits< T >& l) {
    l.min = j.at("min").get< T >();
    l.max = j.at("max").get< T >();
}