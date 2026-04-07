#pragma once

#include <string>
#include "fmt/core.h"

class Version {
private:
    int v_major = 0, v_minor = 0, v_patch = 0;

public:
    Version() = default;
    Version(int v_major, int v_minor, int v_patch) : v_major(v_major), v_minor(v_minor), v_patch(v_patch) {}

    std::string toString() const { return fmt::format("V{}.{}.{}", v_major, v_minor, v_patch); }
};