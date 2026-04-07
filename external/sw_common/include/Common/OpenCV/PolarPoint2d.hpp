#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Math.hpp"
#include "Common/Type/Angle.hpp"

struct PolarPoint2d {
    Angle  angle;
    double distance;

    static inline PolarPoint2d fromCartesian(const cv::Point2d& point) {
        PolarPoint2d polarPoint = {
            .angle    = Angle::fromRadians(std::atan2(point.y, point.x)),
            .distance = magnitude(point),
        };

        return polarPoint;
    }
};

static inline cv::Point2d polarToCartesian(Angle angle) {
    cv::Point2d normalDirection;

    normalDirection.x = cos(angle.getRadians());
    normalDirection.y = sin(angle.getRadians());

    return normalDirection;
}

static inline cv::Point2d polarToCartesian(Angle angle, double distance) { return polarToCartesian(angle) * distance; }

static inline cv::Point2d polarToCartesian(PolarPoint2d polarPoint) {
    return polarToCartesian(polarPoint.angle) * polarPoint.distance;
}
