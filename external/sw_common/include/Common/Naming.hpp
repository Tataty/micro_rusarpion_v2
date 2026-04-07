#pragma once

#include <fmt/core.h>
#include <string>

namespace NAMING {

static inline std::string FROM_TO(const std::string &from, const std::string &to) {
    return fmt::format("{} -> {}", from, to);
}

static inline std::string CHILD_ID(const std::string &baseName, size_t index) {
    return fmt::format("{}#{}", baseName, index);
}

}// namespace NAMING
