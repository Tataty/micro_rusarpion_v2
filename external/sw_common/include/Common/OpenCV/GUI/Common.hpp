#pragma once
#include <opencv2/opencv.hpp>

// TODO: add gui namespace or move to Align
/* cv::Size */
cv::Point topRight(const cv::Size& size);
cv::Point topLeft(const cv::Size& size [[maybe_unused]]);
cv::Point bottomRight(const cv::Size& size);
cv::Point bottomLeft(const cv::Size& size);

cv::Point topCenter(const cv::Size& size);
cv::Point leftCenter(const cv::Size& size);
cv::Point bottomCenter(const cv::Size& size);
cv::Point rightCenter(const cv::Size& size);
cv::Point center(const cv::Size& size);

/* cv::Mat */
cv::Point topRight(const cv::Mat& image);
cv::Point topLeft(const cv::Mat& image [[maybe_unused]]);
cv::Point bottomRight(const cv::Mat& image);
cv::Point bottomLeft(const cv::Mat& image);

cv::Point topCenter(const cv::Mat& image);
cv::Point leftCenter(const cv::Mat& image);
cv::Point bottomCenter(const cv::Mat& image);
cv::Point rightCenter(const cv::Mat& image);
cv::Point center(const cv::Mat& image);

/* cv::Rect */
cv::Point topRight(const cv::Rect& rect);
cv::Point topLeft(const cv::Rect& rect);
cv::Point bottomRight(const cv::Rect& rect);
cv::Point bottomLeft(const cv::Rect& rect);

cv::Point topCenter(const cv::Rect& rect);
cv::Point leftCenter(const cv::Rect& rect);
cv::Point bottomCenter(const cv::Rect& rect);
cv::Point rightCenter(const cv::Rect& rect);
cv::Point center(const cv::Rect& rect);