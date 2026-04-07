#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include "Common/Type/Percent.hpp"

namespace gui {

struct RSSISignal : public iGraphicalElement< cv::Mat > {
public:
    static const int DEFAULT_POWER_LINES = 5;

    struct Config {
        cv::Size                    size;
        cv::Scalar                  frontColor;
        int                         borderThickness = 0;
        std::optional< cv::Scalar > backColor;
        int                         powerLines = DEFAULT_POWER_LINES;
    };

private:
    Config config;
    int    activePowerLines;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        int titheWidth  = busyRect.width / (config.powerLines * 2);
        int fifthWidth  = busyRect.width / config.powerLines;
        int fifthHeight = busyRect.height / config.powerLines;

        int fullThickness = fifthWidth;
        int thickness     = fifthWidth - config.borderThickness;

        cv::Point lineBottom = bottomLeft(busyRect) + cv::Point(titheWidth, 0);
        cv::Point lineTop    = lineBottom - cv::Point(0, fifthHeight);

        for (int i = 0; i < config.powerLines; i++) {

            if (config.backColor.has_value()) {
                cv::rectangle(image,
                              cv::Rect(lineTop + cv::Point(fullThickness / 2, 0),
                                       lineBottom - cv::Point(fullThickness / 2, 0)),
                              config.backColor.value(),
                              -1);
            }

            if (i < activePowerLines) {
                cv::rectangle(image,
                              cv::Rect(lineTop + cv::Point(thickness / 2, config.borderThickness),
                                       lineBottom - cv::Point(thickness / 2, config.borderThickness)),
                              config.frontColor,
                              -1);
            }

            lineBottom.x += titheWidth * 2;
            lineTop.x = lineBottom.x;
            lineTop.y -= fifthHeight;
        }
    }

public:
    RSSISignal(Config config) : config(config) {}

    void setPowerInPercent(const Percent& power) {
        this->activePowerLines = power.getValueByRange(0, config.powerLines);
    }
    const Percent getPowerInPercent() { return Percent::fromRangeValue(activePowerLines, 0, config.powerLines); }

    cv::Size elementSize() override { return config.size; }
};

}// namespace gui
