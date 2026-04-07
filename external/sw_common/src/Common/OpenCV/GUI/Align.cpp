#include "Common/OpenCV/GUI/Align.hpp"

namespace gui {

cv::Point byAlign(const cv::Rect& rect, Align alignment) {
    switch (alignment) {
    case Align::TopRight:
        return topRight(rect);
    case Align::BottomRight:
        return bottomRight(rect);
    case Align::BottomLeft:
        return bottomLeft(rect);
    case Align::Center:
        return center(rect);
    case Align::TopCenter:
        return topCenter(rect);
    case Align::RightCenter:
        return rightCenter(rect);
    case Align::LeftCenter:
        return leftCenter(rect);
    case Align::BottomCenter:
        return bottomCenter(rect);
    case Align::TopLeft:
        return topLeft(rect);
    default:
        return cv::Point(0, 0);
    }
}

}// namespace gui