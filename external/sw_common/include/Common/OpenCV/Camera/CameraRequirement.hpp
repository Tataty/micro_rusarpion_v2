#pragma once

#include "Common/OpenCV/Camera/CameraFormat.hpp"
#include "Common/OpenCV/Common.hpp"

struct CameraRequirement {
    CameraFormat format;
    cv::Size     resolution;
    int          fps;

    auto operator<=>(const CameraRequirement&) const = default;
};

std::string toString(const CameraRequirement& cameraRequirement);
inline bool operator==(const CameraRequirement& lhs, const CameraRequirement& rhs);