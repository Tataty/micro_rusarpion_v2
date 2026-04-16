#pragma once

#include <fmt/core.h>
#include "Common/Type/Angle.hpp"

inline std::string toString(const Angle2& angles) {
    return fmt::format("{:.2f}°:{:.2f}°", angles.yaw.getDegrees(), angles.pitch.getDegrees());
}