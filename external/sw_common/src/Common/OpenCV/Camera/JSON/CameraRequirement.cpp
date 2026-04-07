#include "Common/OpenCV/Camera/CameraRequirement.hpp"

std::string toString(const CameraRequirement& cameraRequirement) {
    std::ostringstream oss;
    oss << cameraRequirement.format.toString() << ": " << toString(cameraRequirement.resolution) << " @ "
        << cameraRequirement.fps << " ";
    return oss.str();
}

inline bool operator==(const CameraRequirement& lhs, const CameraRequirement& rhs) {
    return lhs.format == rhs.format && lhs.resolution == rhs.resolution && lhs.fps == rhs.fps;
}