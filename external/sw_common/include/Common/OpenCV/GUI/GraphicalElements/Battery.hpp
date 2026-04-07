#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include "Common/OpenCV/Common.hpp"
#include "Common/Type/Percent.hpp"

namespace gui {

struct Battery : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        cv::Size                    size;
        cv::Scalar                  frontColor;
        int                         borderThickness;
        std::optional< cv::Scalar > backColor;
    };

private:
    Config  config;
    Percent charge;

    void drawBattery(cv::Mat& image, cv::Rect rect, cv::Scalar color, int thickness) {
        cv::rectangle(image, rect, color, thickness);

        int anode_width  = round(rect.width / 2);
        int anode_height = round(rect.height / 10);

        anode_width = anode_width - anode_width % 2;

        cv::Rect anode_rect(rect.x + (rect.width - anode_width) / 2, rect.y - anode_height, anode_width, anode_height);

        cv::rectangle(image, anode_rect, color, thickness);
    }

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {

        cv::Rect interBatteryRect = expandRect(busyRect, -config.borderThickness * 2);

        if (config.backColor.has_value()) {
            drawBattery(image, busyRect, config.backColor.value(), cv::FILLED);
        }
        drawBattery(image, interBatteryRect, config.frontColor, config.borderThickness);

        cv::Rect chargeRect;
        chargeRect.height = interBatteryRect.height * charge.getNormalize();
        chargeRect.width  = interBatteryRect.width;
        chargeRect.x      = interBatteryRect.x;
        chargeRect.y      = interBatteryRect.y + (interBatteryRect.height - chargeRect.height);

        cv::rectangle(image, chargeRect, config.frontColor, cv::FILLED);
    }

public:
    Battery(const Config& config) : config(config) {}

    void          setCharge(const Percent& charge) { this->charge = charge; }
    const Percent getCharge() { return charge; }

    cv::Size elementSize() override { return config.size; }
};

}// namespace gui
