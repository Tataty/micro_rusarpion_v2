#pragma once
#include "Common/OpenCV/GUI/GraphicalElements/Label.hpp"
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include <opencv2/freetype.hpp>

namespace gui {

struct PitchTags : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        Label::Config labelConfig;
        int           tagWidth;
    };

    struct Tag {
        int verticalPixels;
        int distanceInMeters;
    };

private:
    Config config;
    Label  label;

    std::vector< Tag > tags;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {

        int lastVerticalPixels = 0;

        for (const auto& tag : tags) {
            if (tag.verticalPixels - lastVerticalPixels < config.labelConfig.getTextHeight())
                continue;

            lastVerticalPixels = tag.verticalPixels;

            /* Line */
            cv::Point tagLeftPosition = topLeft(busyRect);
            tagLeftPosition.y += tag.verticalPixels;

            cv::Point tagRightPosition = topLeft(busyRect);
            tagRightPosition.x += config.tagWidth;
            tagRightPosition.y += tag.verticalPixels;

            if (config.labelConfig.borderColor.has_value()) {
                cv::rectangle(image,
                              tagLeftPosition,
                              tagRightPosition,
                              config.labelConfig.borderColor.value(),
                              config.labelConfig.textThickness + config.labelConfig.textBorderThickness);
            }

            cv::rectangle(image,
                          tagLeftPosition,
                          tagRightPosition,
                          config.labelConfig.frontColor,
                          config.labelConfig.textThickness);

            label.setText(std::to_string(tag.distanceInMeters) + "m");
            label.setPosition(tagRightPosition);
            label.draw(image);
        }
    }

public:
    PitchTags(Config config) : config(config), label(config.labelConfig) { label.setAlignment(Align::LeftCenter); }

    cv::Size elementSize() override { return cv::Size(config.tagWidth, config.tagWidth); }

    void               setTags(std::vector< Tag > tags) { this->tags = tags; }
    std::vector< Tag > getTags() { return tags; }
};

}// namespace gui
