#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

inline std::string toString(const std::vector< double >& listOfDouble, int precision = 3) {
    if (listOfDouble.empty()) {
        return "[]";
    }

    std::ostringstream oss;
    oss << "[";
    oss << std::fixed << std::setprecision(precision);

    for (size_t i = 0; i < listOfDouble.size(); ++i) {
        oss << listOfDouble[i];
        if (i < listOfDouble.size() - 1) {
            oss << ", ";
        }
    }

    oss << "]";
    return oss.str();
}