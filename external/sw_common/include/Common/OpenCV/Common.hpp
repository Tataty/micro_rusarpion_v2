#pragma once
#include <opencv2/freetype.hpp>
#include <opencv2/opencv.hpp>

#include <fmt/format.h>
#include <iomanip>
#include <sstream>
#include <string>

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
bool   isCollisionRect(const cv::Rect& a, const cv::Rect& b);
double innerRadiusRect(const cv::Rect& rect);

cv::Rect expandRect(const cv::Rect& rect, int expand);
cv::Rect expandRect(const cv::Rect& rect, cv::Size expand);
cv::Rect expandRect(const cv::Size& sizeRect, int expand);
cv::Rect expandRect(const cv::Size& sizeRect, cv::Size expand);

cv::Rect clampRect(const cv::Rect& inner, const cv::Rect& outer);
void     fitInside(cv::Rect& smaller, const cv::Rect& larger);

/* cv::Mat */
cv::Size imageSize(const cv::Mat& image);

cv::Mat makeZeroMatBGR(cv::Size size);
cv::Mat makeZeroMatYUV_I420(cv::Size size);

bool areMatsEqual(const cv::Mat& a, const cv::Mat& b);

void copyResize(cv::InputArray src, cv::OutputArray dst, cv::Size dsize);

/* Common */
template < typename T > cv::Rect_< T > toRect(const cv::Size_< T >& size) { return cv::Rect_< T >(0, 0, size.width, size.height); }