#pragma once
#include <opencv2/opencv.hpp>

inline cv::Point rectCenter(const cv::Rect& rect) {
    return cv::Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}

inline double magnitude(cv::Point2d point) { return std::sqrt(point.x * point.x + point.y * point.y); }
inline double magnitude(cv::Point3d point) {
    return std::sqrt(point.x * point.x + point.y * point.y + point.z * point.z);
}
