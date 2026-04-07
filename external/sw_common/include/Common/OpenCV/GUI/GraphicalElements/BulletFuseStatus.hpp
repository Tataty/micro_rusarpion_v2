#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include <math.h>
#include "Common/OpenCV/Common.hpp"

namespace gui {

struct BulletFuseStatus : public iGraphicalElement< cv::Mat > {
public:
    static constexpr int COUNT_PARTS = 3;

    struct Config {
        cv::Size   size;
        cv::Scalar frontColor;
        int        borderThickness = 0;

        std::optional< cv::Scalar > backColor;
    };

private:
    Config config;
    bool   isActive = false;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {

        cv::Rect frontRects[COUNT_PARTS];

        double partWidth[]  = { 0.2, 0.3, 0.4 };
        double partHeight[] = { 0.55, 0.95, 1.0 };

        for (size_t i = 0; i < COUNT_PARTS; i++) {

            frontRects[i].x      = floor(busyRect.x + busyRect.width * partWidth[i]);
            frontRects[i].y      = floor(busyRect.y + busyRect.height * (1 - partHeight[i]));
            frontRects[i].width  = ceil(busyRect.width * (1 - partWidth[i] * 2));
            frontRects[i].height = ceil(busyRect.height * partHeight[i]);

            if (i > 0)
                frontRects[i].height -= floor(busyRect.height * partHeight[i - 1]);

            if (config.backColor.has_value()) {
                cv::Rect backRect = expandRect(frontRects[i], config.borderThickness);
                cv::rectangle(image, backRect, config.backColor.value(), cv::FILLED);
            }
        }

        for (size_t i = 0; i < COUNT_PARTS; i++) {
            cv::rectangle(image, frontRects[i], config.frontColor, cv::FILLED);
        }

        if (isActive == false) {

            int lineThickness = busyRect.width * 0.06;

            cv::Point leftLinePoint = leftCenter(busyRect);
            leftLinePoint.y -= busyRect.height * 0.1;

            cv::Point rightLinePoint = bottomRight(busyRect);
            leftLinePoint.y -= busyRect.height * 0.05;

            if (config.backColor.has_value()) {
                cv::line(image,
                         leftLinePoint,
                         rightLinePoint,
                         config.backColor.value(),
                         lineThickness + config.borderThickness * 1.5);
            }
            cv::line(image, leftLinePoint, rightLinePoint, config.frontColor, lineThickness);
        }
    }

public:
    BulletFuseStatus(Config config) : config(config) {}

    void setStatus(bool isActive) { this->isActive = isActive; }
    bool getStatus() { return isActive; }

    cv::Size elementSize() override { return config.size; }
};

}// namespace gui
/*
   0.0 0.2   0.5  0.9
    |---|-----|----|--|--|----|-----|---|
    |              _______              |
0.9 -------------------------------------
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
    |         |               |         |
0.6 -------------------------------------
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
    |   |                           |   |
0.0 -------------------------------------
*/