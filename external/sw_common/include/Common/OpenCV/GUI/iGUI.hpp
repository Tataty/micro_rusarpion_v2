#pragma once
#include <opencv2/freetype.hpp>

#include "Common/OpenCV/GUI/GraphicalElements/Label.hpp"
#include "Common/Text.hpp"
#include "Common/Utils/TimeDelayMeter.hpp"

class iGUI {
public:
    struct Color {
        cv::Scalar primary;
        cv::Scalar secondary;

        cv::Scalar enabled;
        cv::Scalar disabled;

        cv::Scalar blink;

        auto operator<=>(const Color&) const = default;
    };

    struct Visual {
        int lineThickness;
        int borderThickness;

        int smallFontHeight;
        int mediumFontHeight;

        CVFont font;

        auto operator<=>(const Visual&) const = default;
    };

    struct Style {
        Color  color;
        Visual visual;

        auto operator<=>(const Style&) const = default;
    };

protected:
    static std::string formatText(const std::string& text_utf8) {
        std::u16string text  = utf8To16(text_utf8);
        std::u16string upper = toUpper(text);
        return utf16To8(upper);
    }
    static gui::Label::Config getDefaultLabelConfig(const Style& style) {
        return gui::Label::Config{
            .fontHeight          = style.visual.mediumFontHeight,
            .textThickness       = style.visual.lineThickness,
            .frontColor          = style.color.primary,
            .font                = style.visual.font,
            .textBorderThickness = style.visual.borderThickness,
            .borderColor         = style.color.secondary,
        };
    };
    static gui::Label::Config getSmallLabelConfig(const Style& style) {
        return gui::Label::Config{
            .fontHeight          = style.visual.smallFontHeight,
            .textThickness       = style.visual.lineThickness,
            .frontColor          = style.color.primary,
            .font                = style.visual.font,
            .textBorderThickness = style.visual.borderThickness,
            .borderColor         = style.color.secondary,
        };
    };

public:
    virtual ~iGUI() = default;

    virtual void draw(cv::Mat& image) = 0;
};