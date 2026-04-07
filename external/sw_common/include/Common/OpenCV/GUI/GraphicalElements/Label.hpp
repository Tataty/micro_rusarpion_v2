#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include "Common/OpenCV/CVFont.hpp"

namespace gui {

struct Label : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        int        fontHeight;
        int        textThickness;
        cv::Scalar frontColor;

        CVFont font;

        int                         textBorderThickness = 0;
        std::optional< cv::Scalar > borderColor;
        int                         textLineStyle = cv::LINE_8;

        int getTextHeight() {
            int      baseLine;
            cv::Size textSize = font.getTextSize(CVFont::TEXT_FOR_CALCULATE_SIZE, fontHeight, textThickness, &baseLine);

            return textSize.height;
        }
    };

private:
    const Config config;
    std::string  text;
    int          uniformTextHeight;
    int          uniformBaseLine;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {
        cv::Point textPosition(busyRect.x, busyRect.y - uniformBaseLine);

        if (config.borderColor.has_value() && config.textBorderThickness > 0) {
            config.font.putText(image,
                                text,
                                textPosition,
                                config.fontHeight,
                                config.borderColor.value(),
                                config.textThickness + config.textBorderThickness,
                                config.textLineStyle,
                                false);
        }

        config.font.putText(
                image, text, textPosition, config.fontHeight, config.frontColor, -1, config.textLineStyle, false);
    }

public:
    Label(const Config& config) : config(config) {
        uniformTextHeight = config.font
                                    .getTextSize(CVFont::TEXT_FOR_CALCULATE_SIZE,
                                                 config.fontHeight,
                                                 config.textThickness,
                                                 &uniformBaseLine)
                                    .height;
    }

    void               setText(const std::string& text) { this->text = text; }
    const std::string& getText() { return text; }

    cv::Size elementSize() override {
        /* Calculate Label Size */
        int      baseLine;
        cv::Size textSize;

        textSize        = config.font.getTextSize(text, config.fontHeight, config.textThickness, &baseLine);
        textSize.height = uniformTextHeight;

        textSize.height += baseLine;

        /* Busy Rect */
        cv::Size size;
        size.width  = textSize.width;
        size.height = textSize.height;

        return size;
    }
};

}// namespace gui
