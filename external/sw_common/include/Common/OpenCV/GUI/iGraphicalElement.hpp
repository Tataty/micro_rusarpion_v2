#pragma once

#include <optional>
#include "Common/OpenCV/Common.hpp"
#include "Common/OpenCV/GUI/Align.hpp"
#include "Common/OpenCV/GUI/Common.hpp"

namespace gui {

template < typename TImage > class iGraphicalElement {
protected:
    cv::Point                 position;
    Align                     alignment = Align::TopLeft;
    std::optional< cv::Size > correctSize;

    cv::Rect getAlignRect(cv::Size size) {
        cv::Rect rect;
        rect.width  = size.width;
        rect.height = size.height;

        /* Apply Alignment */
        switch (alignment) {
        case Align::TopRight: {
            rect.x = position.x - rect.width;
            rect.y = position.y;
        } break;
        case Align::BottomRight: {
            rect.x = position.x - rect.width;
            rect.y = position.y - rect.height;
        } break;
        case Align::BottomLeft: {
            rect.x = position.x;
            rect.y = position.y - rect.height;
        } break;
        case Align::Center: {
            rect.x = position.x - rect.width / 2;
            rect.y = position.y - rect.height / 2;
        } break;
        case Align::TopCenter: {
            rect.x = position.x - rect.width / 2;
            rect.y = position.y;
        } break;
        case Align::RightCenter: {
            rect.x = position.x - rect.width;
            rect.y = position.y - rect.height / 2;
        } break;
        case Align::LeftCenter: {
            rect.x = position.x;
            rect.y = position.y - rect.height / 2;
        } break;
        case Align::BottomCenter: {
            rect.x = position.x - rect.width / 2;
            rect.y = position.y - rect.height;
        } break;
        case Align::TopLeft: {
            rect.x = position.x;
            rect.y = position.y;
        } break;
        default:
            break;
        }

        return rect;
    }

    virtual void drawElement(TImage& image, cv::Rect busyRect) = 0;

public:
    virtual ~iGraphicalElement() = default;

    void  setPosition(cv::Point position) { this->position = position; }
    void  setAlignment(Align alignment) { this->alignment = alignment; }
    Align getAlignment() { return alignment; }

    virtual cv::Size elementSize() = 0;

    void draw(TImage& image) { drawElement(image, getBusySpace()); }

    cv::Rect getBusySpace() {
        cv::Rect busyRect = getAlignRect(elementSize());

        if (correctSize.has_value()) {
            /*  Correct In Size */
            if (busyRect.x < 0)
                busyRect.x = 0;
            if (busyRect.y < 0)
                busyRect.y = 0;
            if (busyRect.x + busyRect.width > correctSize->width)
                busyRect.x = correctSize->width - busyRect.width;
            if (busyRect.y + busyRect.height > correctSize->height)
                busyRect.y = correctSize->height - busyRect.height;
        }

        return busyRect;
    }

    void setCorrectInSize(cv::Size size) { correctSize = size; }
};

}// namespace gui