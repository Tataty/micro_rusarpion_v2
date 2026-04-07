#include "Common/OpenCV/Common.hpp"

/* cv::Size */
bool operator==(const cv::MatSize& ms, const cv::Size& s) { return (ms[1] == s.width) && (ms[0] == s.height); }
bool operator==(const cv::Size& s, const cv::MatSize& ms) { return ms == s; }

std::string   toString(const cv::Size& size) { return fmt::format("{}x{}", size.width, size.height); }
std::ostream& operator<<(std::ostream& os, const cv::Size& s) { return os << s.width << "x" << s.height; }
cv::Size      operator-(const cv::Size& size) { return cv::Size(-size.width, -size.height); }

/* cv::Scalar */
cv::Scalar hexToScalar(const std::string& hex) {
    if (hex.length() != 7 || hex[0] != '#') {
        throw std::runtime_error("Incorrect format HEX color: " + hex);
    }

    std::string redHex   = hex.substr(1, 2);
    std::string greenHex = hex.substr(3, 2);
    std::string blueHex  = hex.substr(5, 2);

    unsigned int      r, g, b;
    std::stringstream ss;
    ss << std::hex << redHex << " " << greenHex << " " << blueHex;
    ss >> r >> g >> b;

    return { static_cast< double >(b), static_cast< double >(g), static_cast< double >(r) };
}

/* cv::Point */
std::string   toString(const cv::Point& point) { return fmt::format("({}:{})", point.x, point.y); }
std::ostream& operator<<(std::ostream& os, const cv::Point& point) {
    return os << '(' << point.x << ':' << point.y << ')';
}
cv::Point operator-(const cv::Point& point) { return cv::Point(-point.x, -point.y); }

/* cv::Rect */
bool   isCollisionRect(const cv::Rect& a, const cv::Rect& b) { return (a & b).area() > 0; }
double innerRadiusRect(const cv::Rect& rect) { return std::min(rect.width, rect.height) / 2; }

cv::Rect expandRect(const cv::Rect& rect, int expand) {
    return cv::Rect(rect.x - expand, rect.y - expand, rect.width + expand * 2, rect.height + expand * 2);
}
cv::Rect expandRect(const cv::Rect& rect, cv::Size expand) {
    return cv::Rect(rect.x - expand.width,
                    rect.y - expand.height,
                    rect.width + expand.width * 2,
                    rect.height + expand.height * 2);
}
cv::Rect expandRect(const cv::Size& sizeRect, int expand) {
    return cv::Rect(-expand, -expand, sizeRect.width + expand * 2, sizeRect.height + expand * 2);
}
cv::Rect expandRect(const cv::Size& sizeRect, cv::Size expand) {
    return cv::Rect(
            -expand.width, -expand.height, sizeRect.width + expand.width * 2, sizeRect.height + expand.height * 2);
}

cv::Rect clampRect(const cv::Rect& inner, const cv::Rect& outer) { return inner & outer; }
void     fitInside(cv::Rect& smaller, const cv::Rect& larger) {
    if (smaller.width > larger.width || smaller.height > larger.height)
        throw std::runtime_error("Larger rectangle is smaller than the smaller rectangle");

    if (smaller.x < larger.x) {
        smaller.x = larger.x;
    } else if (smaller.x + smaller.width > larger.x + larger.width) {
        smaller.x = larger.x + larger.width - smaller.width;
    }

    if (smaller.y < larger.y) {
        smaller.y = larger.y;
    } else if (smaller.y + smaller.height > larger.y + larger.height) {
        smaller.y = larger.y + larger.height - smaller.height;
    }
}

/* cv::Image */
cv::Size imageSize(const cv::Mat& image) { return cv::Size(image.cols, image.rows); }

cv::Mat makeZeroMatBGR(cv::Size size) { return cv::Mat::zeros(size.height, size.width, CV_8UC3); }

cv::Mat makeZeroMatYUV_I420(cv::Size size) {
    return cv::Mat::zeros(size.height + (size.height / 2), size.width, CV_8UC1);
}

bool areMatsEqual(const cv::Mat& a, const cv::Mat& b) {
    if (a.size() != b.size() || a.type() != b.type()) {
        return false;
    }

    cv::Mat diff;
    cv::compare(a, b, diff, cv::CMP_NE);
    int nonzero = cv::countNonZero(diff.reshape(1));

    return (nonzero == 0);
}

void copyResize(cv::InputArray src, cv::OutputArray dst, cv::Size dsize) {
    if (src.size() == dsize)
        src.copyTo(dst);
    else
        cv::resize(src, dst, dsize);
}