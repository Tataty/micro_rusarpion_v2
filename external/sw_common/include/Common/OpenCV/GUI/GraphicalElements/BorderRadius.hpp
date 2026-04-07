#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include <opencv2/freetype.hpp>

namespace gui {

struct BorderRadius : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        int borderRadius = 0;
    };

private:
    Config config;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        interElement->setPosition(center(busyRect));
        interElement->setAlignment(Align::Center);

        interElement->draw(image);
    }

public:
    std::shared_ptr< iGraphicalElement< cv::Mat > > interElement;

    BorderRadius(Config config, std::shared_ptr< iGraphicalElement< cv::Mat > > interElement)
        : config(config), interElement(interElement) {}

    cv::Size elementSize() override {
        cv::Size size = interElement->elementSize();
        size.width += config.borderRadius * 2;
        size.height += config.borderRadius * 2;

        return size;
    }
};

}// namespace gui
