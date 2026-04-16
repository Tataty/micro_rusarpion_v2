#pragma once

#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

namespace gui {

struct Cross : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        cv::Size   size;
        cv::Scalar frontColor;
        int        lineThickness;
    };

private:
    Config config;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        cv::line(image, topLeft(busyRect), bottomRight(busyRect), config.frontColor, config.lineThickness); /* \ */
        cv::line(image, topRight(busyRect), bottomLeft(busyRect), config.frontColor, config.lineThickness); /* / */
    }

public:
    Cross(Config config) : config(config) {}

    void     setSize(cv::Size size) { config.size = size; }
    cv::Size getSize() { return config.size; }
    void     setColor(cv::Scalar color) { config.frontColor = color; }

    cv::Size elementSize() override {
        return cv::Size(config.size.width + config.lineThickness, config.size.height + config.lineThickness);
    }
};

}// namespace gui
