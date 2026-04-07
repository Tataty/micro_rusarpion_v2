#include <opencv2/opencv.hpp>

/* cv::Size */
cv::Point topRight(const cv::Size& size) { return cv::Point(size.width, 0); }
cv::Point topLeft(const cv::Size& size [[maybe_unused]]) { return cv::Point(0, 0); }
cv::Point bottomRight(const cv::Size& size) { return cv::Point(size.width, size.height); }
cv::Point bottomLeft(const cv::Size& size) { return cv::Point(0, size.height); }

cv::Point topCenter(const cv::Size& size) { return cv::Point(size.width / 2, 0); }
cv::Point leftCenter(const cv::Size& size) { return cv::Point(0, size.height / 2); }
cv::Point bottomCenter(const cv::Size& size) { return cv::Point(size.width / 2, size.height); }
cv::Point rightCenter(const cv::Size& size) { return cv::Point(size.width, size.height / 2); }
cv::Point center(const cv::Size& size) { return cv::Point(size.width / 2, size.height / 2); }

/* cv::Mat */
cv::Point topRight(const cv::Mat& image) { return cv::Point(image.cols, 0); }
cv::Point topLeft(const cv::Mat& image [[maybe_unused]]) { return cv::Point(0, 0); }
cv::Point bottomRight(const cv::Mat& image) { return cv::Point(image.cols, image.rows); }
cv::Point bottomLeft(const cv::Mat& image) { return cv::Point(0, image.rows); }

cv::Point topCenter(const cv::Mat& image) { return cv::Point(image.cols / 2, 0); }
cv::Point leftCenter(const cv::Mat& image) { return cv::Point(0, image.rows / 2); }
cv::Point bottomCenter(const cv::Mat& image) { return cv::Point(image.cols / 2, image.rows); }
cv::Point rightCenter(const cv::Mat& image) { return cv::Point(image.cols, image.rows / 2); }
cv::Point center(const cv::Mat& image) { return cv::Point(image.cols / 2, image.rows / 2); }

/* cv::Rect */
cv::Point topRight(const cv::Rect& rect) { return cv::Point(rect.x + rect.width, rect.y); }
cv::Point topLeft(const cv::Rect& rect) { return cv::Point(rect.x, rect.y); }
cv::Point bottomRight(const cv::Rect& rect) { return cv::Point(rect.x + rect.width, rect.y + rect.height); }
cv::Point bottomLeft(const cv::Rect& rect) { return cv::Point(rect.x, rect.y + rect.height); }

cv::Point topCenter(const cv::Rect& rect) { return cv::Point(rect.x + rect.width / 2, rect.y); }
cv::Point leftCenter(const cv::Rect& rect) { return cv::Point(rect.x, rect.y + rect.height / 2); }
cv::Point bottomCenter(const cv::Rect& rect) { return cv::Point(rect.x + rect.width / 2, rect.y + rect.height); }
cv::Point rightCenter(const cv::Rect& rect) { return cv::Point(rect.x + rect.width, rect.y + rect.height / 2); }
cv::Point center(const cv::Rect& rect) { return cv::Point(rect.x + rect.width / 2, rect.y + rect.height / 2); }