#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Common.hpp"
#include "Common/Type/Angle.hpp"

static Angle angleBetweenPointVector(const cv::Point3f& a, const cv::Point3f& b) {
    double dot  = a.dot(b);
    double magA = cv::magnitude(a);
    double magB = cv::magnitude(b);

    double angleInRadians = std::acos(dot / (magA * magB));

    return Angle::fromRadians(angleInRadians);
}