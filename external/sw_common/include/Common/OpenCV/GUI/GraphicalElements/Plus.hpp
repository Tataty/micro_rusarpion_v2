#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

namespace gui {

struct Plus : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        int        radius;
        int        freeSpace;
        int        lineThickness;
        cv::Scalar frontColor;

        int        borderThickness;
        cv::Scalar backColor;

        bool isCenterPoint = false;
    };

private:
    Config config;

    void drawPlus(cv::Mat& image, cv::Point position, cv::Scalar color, int thickness) {
        /* | */
        cv::line(image,
                 position + cv::Point(0, -config.radius),
                 position + cv::Point(0, -config.freeSpace),
                 color,
                 thickness);
        cv::line(image,
                 position + cv::Point(0, config.freeSpace),
                 position + cv::Point(0, config.radius),
                 color,
                 thickness);

        /* - */
        cv::line(image,
                 position + cv::Point(-config.radius, 0),
                 position + cv::Point(-config.freeSpace, 0),
                 color,
                 thickness);
        cv::line(image,
                 position + cv::Point(config.freeSpace, 0),
                 position + cv::Point(config.radius, 0),
                 color,
                 thickness);

        if (config.isCenterPoint) {
            /* * */
            cv::line(image, position, position, color, thickness);
        }
    }

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        cv::Point plusPosition = center(busyRect);

        if (config.borderThickness > 0) {
            drawPlus(image, plusPosition, config.backColor, config.lineThickness + config.borderThickness);
        }

        drawPlus(image, plusPosition, config.frontColor, config.lineThickness);
    }

public:
    Plus(Config config) : config(config) {}

    cv::Size elementSize() override {
        return cv::Size(config.radius * 2 + config.borderThickness + config.lineThickness,
                        config.radius * 2 + config.borderThickness + config.lineThickness);
    }
};

}// namespace gui
