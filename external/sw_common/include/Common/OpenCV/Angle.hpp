#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Math.hpp"
#include "Common/Type/Angle.hpp"

static Angle angleBetweenPointVector(const cv::Point3f &a, const cv::Point3f &b) {
    double dot  = a.dot(b);
    double magA = magnitude(a);
    double magB = magnitude(b);

    double angleInRadians = std::acos(dot / (magA * magB));

    return Angle::fromRadians(angleInRadians);
}