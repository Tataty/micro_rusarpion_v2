#pragma once

#include "Common/OpenCV/GUI/Common.hpp"

namespace gui {

enum class Align {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Center,
    LeftCenter,
    TopCenter,
    RightCenter,
    BottomCenter
};

cv::Point byAlign(const cv::Rect& rect, Align alignment);

}// namespace gui