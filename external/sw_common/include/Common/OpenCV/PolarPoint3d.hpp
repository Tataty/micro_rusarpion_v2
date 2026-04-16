#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Common.hpp"
#include "Common/Type/Angle.hpp"

struct PolarPoint3d {
    Angle2 angles;
    double distance;

    static inline PolarPoint3d fromCartesian(const cv::Point3d& point) {
        const double horizontal_distance = std::sqrt(point.x * point.x + point.z * point.z);

        PolarPoint3d polarPoint = {
            .angles = {
                .yaw   = Angle::fromRadians(std::atan2(point.z, point.x)),
                .pitch = Angle::fromRadians(std::atan2(point.y, horizontal_distance)),
            },
            .distance = cv::magnitude(point),
        };

        return polarPoint;
    }
};

static inline cv::Point3d polarToCartesian(Angle2 angles) {
    cv::Point3d normalDirection;

    normalDirection.x = cos(angles.yaw.getRadians()) * cos(angles.pitch.getRadians());
    normalDirection.y = sin(angles.pitch.getRadians());
    normalDirection.z = sin(angles.yaw.getRadians()) * cos(angles.pitch.getRadians());

    return normalDirection;
}

static inline cv::Point3d polarToCartesian(Angle2 angles, double distance) {
    return polarToCartesian(angles) * distance;
}

static inline cv::Point3d polarToCartesian(PolarPoint3d polarPoint) {
    return polarToCartesian(polarPoint.angles) * polarPoint.distance;
}
