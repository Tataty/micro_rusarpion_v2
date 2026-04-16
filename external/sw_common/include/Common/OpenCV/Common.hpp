#pragma once
#include <opencv2/freetype.hpp>
#include <opencv2/opencv.hpp>

#include <fmt/format.h>
#include <iomanip>
#include <sstream>
#include <string>

#include "Common/OpenCV/GUI/Common.hpp"

/* cv::Size */
bool operator==(const cv::MatSize& ms, const cv::Size& s);
bool operator==(const cv::Size& s, const cv::MatSize& ms);

std::string   toString(const cv::Size& size);
std::ostream& operator<<(std::ostream& os, const cv::Size& s);
cv::Size      operator-(const cv::Size& size);

template < typename T >
concept IsSize = requires(T v) {
    v.width;
    v.height;
};

template < IsSize T1, IsSize T2 > auto elementWiseProduct(const T1& a, const T2& b) { return T1(a.width * b.width, a.height * b.height); }

/* cv::Scalar */
static inline const cv::Scalar WHITE_COLOR = cv::Scalar(255, 255, 255);

cv::Scalar hexToScalar(const std::string& hex);

/* cv::Point */
std::string   toString(const cv::Point& point);
std::ostream& operator<<(std::ostream& os, const cv::Point& point);
cv::Point     operator-(const cv::Point& size);

template < typename T >
concept IsPoint = requires(T v) {
    v.x;
    v.y;
};

template < IsPoint T1, IsPoint T2 > auto elementWiseProduct(const T1& a, const T2& b) { return T1(a.x * b.x, a.y * b.y); }

/* cv::Rect */
std::string toString(const cv::Rect& rect);

double innerRadiusRect(const cv::Rect& rect);

cv::Rect expandRect(const cv::Rect& rect, int expand);
cv::Rect expandRect(const cv::Rect& rect, cv::Size expand);
cv::Rect expandRect(const cv::Size& sizeRect, int expand);
cv::Rect expandRect(const cv::Size& sizeRect, cv::Size expand);

cv::Rect clampRect(const cv::Rect& inner, const cv::Rect& outer);
void     fitInside(cv::Rect& smaller, const cv::Rect& larger);

cv::Point rectCenter(const cv::Rect& rect);
cv::Rect  getRectInCenter(const cv::Point& point, const cv::Size& size);

double rectOverlap(const cv::Rect& rect1, const cv::Rect& rect2);

void correctRectInSize(cv::Rect& rect, const cv::Size& size);

/* cv::Mat */
cv::Size imageSize(const cv::Mat& image);

cv::Mat makeZeroMatBGR(cv::Size size);
cv::Mat makeZeroMatYUV_I420(cv::Size size);

bool areMatsEqual(const cv::Mat& a, const cv::Mat& b);

void copyResize(cv::InputArray src, cv::OutputArray dst, cv::Size dsize);

inline cv::Point centerImage(const cv::Mat& image) { return cv::Point(image.cols / 2, image.rows / 2); }

/* Common */
template < typename T > cv::Rect_< T > toRect(const cv::Size_< T >& size) { return cv::Rect_< T >(0, 0, size.width, size.height); }

template < typename T > cv::Point_< T > toPoint(const cv::Size_< T >& size) { return cv::Point_< T >(size.width, size.height); }

namespace cv {

template < typename T > struct Circle_ {
    Point_< T > center;
    T           radius;
};

using Circle2i = Circle_< int >;
using Circle2d = Circle_< double >;

template < typename T1, typename T2 > bool isCollision(const Circle_< T1 >& circle, const Rect_< T2 >& rect) {
    T1 closestX = std::clamp(circle.center.x, static_cast< T1 >(rect.x), static_cast< T1 >(rect.x + rect.width));
    T1 closestY = std::clamp(circle.center.y, static_cast< T1 >(rect.y), static_cast< T1 >(rect.y + rect.height));

    T1 distanceX = circle.center.x - closestX;
    T1 distanceY = circle.center.y - closestY;

    T1 distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared <= (circle.radius * circle.radius);
}

bool isCollision(const Rect& a, const Rect& b);

template < typename T > double rectRadius(const Rect_< T >& rect) { return std::min(rect.width, rect.height) / 2.d; }

template < typename T > double magnitude(const Point_< T >& point) { return std::sqrt(point.x * point.x + point.y * point.y); }
template < typename T > double magnitude(const Point3_< T >& point) { return std::sqrt(point.x * point.x + point.y * point.y + point.z * point.z); }

}// namespace cv