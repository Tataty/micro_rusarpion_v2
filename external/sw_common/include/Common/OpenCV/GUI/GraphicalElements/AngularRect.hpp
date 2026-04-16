#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

namespace gui {

struct AngularRect : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        cv::Size halfSize;
        int      angularRectLength;

        int        lineThickness;
        cv::Scalar frontColor;

        int        borderThickness = 0;
        cv::Scalar backColor;
    };

private:
    Config config;

    void drawAngularRect(cv::Mat& image, cv::Point center, cv::Scalar color, int thickness) {
        // |``
        cv::line(image,
                 center + cv::Point(-config.halfSize.width, -config.halfSize.height),
                 center + cv::Point(-config.halfSize.width + config.angularRectLength, -config.halfSize.height),
                 color,
                 thickness);
        cv::line(image,
                 center + cv::Point(-config.halfSize.width, -config.halfSize.height),
                 center + cv::Point(-config.halfSize.width, -config.halfSize.height + config.angularRectLength),
                 color,
                 thickness);

        // |..
        cv::line(image,
                 center + cv::Point(-config.halfSize.width, config.halfSize.height),
                 center + cv::Point(-config.halfSize.width + config.angularRectLength, config.halfSize.height),
                 color,
                 thickness);
        cv::line(image,
                 center + cv::Point(-config.halfSize.width, config.halfSize.height),
                 center + cv::Point(-config.halfSize.width, config.halfSize.height - config.angularRectLength),
                 color,
                 thickness);

        // ``|
        cv::line(image,
                 center + cv::Point(config.halfSize.width, -config.halfSize.height),
                 center + cv::Point(config.halfSize.width - config.angularRectLength, -config.halfSize.height),
                 color,
                 thickness);
        cv::line(image,
                 center + cv::Point(config.halfSize.width, -config.halfSize.height),
                 center + cv::Point(config.halfSize.width, -config.halfSize.height + config.angularRectLength),
                 color,
                 thickness);

        // ..|
        cv::line(image,
                 center + cv::Point(config.halfSize.width, config.halfSize.height),
                 center + cv::Point(config.halfSize.width - config.angularRectLength, config.halfSize.height),
                 color,
                 thickness);
        cv::line(image,
                 center + cv::Point(config.halfSize.width, config.halfSize.height),
                 center + cv::Point(config.halfSize.width, config.halfSize.height - config.angularRectLength),
                 color,
                 thickness);
    }

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        cv::Point centerPosition = center(busyRect);

        if (config.borderThickness > 0) {
            drawAngularRect(image, centerPosition, config.backColor, config.lineThickness + config.borderThickness);
        }

        drawAngularRect(image, centerPosition, config.frontColor, config.lineThickness);
    }

public:
    AngularRect(Config config) : config(config) {}

    cv::Size elementSize() override { return cv::Size(config.halfSize.width * 2, config.halfSize.height * 2); }

    void     setHalfSize(cv::Size size) { config.halfSize = size; }
    cv::Size getHalfSize() { return config.halfSize; }

    void setFrontColor(cv::Scalar color) { config.frontColor = color; }
};

}// namespace gui