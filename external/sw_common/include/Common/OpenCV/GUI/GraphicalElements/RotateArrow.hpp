#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include "Common/OpenCV/PolarPoint2d.hpp"
#include "Common/Type/Angle.hpp"

namespace gui {

struct RotateArrow : public iGraphicalElement< cv::Mat > {
public:
    const Angle ARROW_HALFANGLE = Angle::fromDegrees(60);

    struct Config {
        int        radius;
        cv::Scalar frontColor;
        int        borderThickness = 0;
        Angle      offsetRotateAngle;

        std::optional< cv::Scalar > backColor;
    };

private:
    Config config;
    Angle  rotateAngle;

    void drawArrow(cv::Mat& image, cv::Point center, int radius, cv::Scalar color) {

        PolarPoint2d polarFront = {
            .angle    = rotateAngle,
            .distance = static_cast< double >(radius),
        };

        PolarPoint2d polarRight = {
            .angle    = rotateAngle + Angle::fromDegrees(180) - ARROW_HALFANGLE,
            .distance = static_cast< double >(radius) / 2,
        };

        PolarPoint2d polarLeft = {
            .angle    = rotateAngle + Angle::fromDegrees(180) + ARROW_HALFANGLE,
            .distance = static_cast< double >(radius) / 2,
        };

        std::vector< cv::Point > vertices;

        vertices.push_back(polarToCartesian(polarFront) + cv::Point2d(center));
        vertices.push_back(polarToCartesian(polarLeft) + cv::Point2d(center));
        vertices.push_back(center);
        vertices.push_back(polarToCartesian(polarRight) + cv::Point2d(center));

        for (cv::Point& vertex : vertices) {
            vertex -= cv::Point(polarToCartesian(polarFront)) / 3;
        }

        cv::fillPoly(image, vertices, color);
    }

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {

        if (config.backColor.has_value()) {
            drawArrow(image, center(busyRect), config.radius, config.backColor.value());
        }

        drawArrow(image, center(busyRect), config.radius - config.borderThickness * 2, config.frontColor);
    }

public:
    RotateArrow(Config config) : config(config) { this->rotateAngle = config.offsetRotateAngle; }

    void        setRotateAngle(const Angle& angle) { this->rotateAngle = angle + config.offsetRotateAngle; }
    const Angle getRotateAngle() { return rotateAngle; }

    cv::Size elementSize() override { return cv::Size(config.radius * 2, config.radius * 2); }
};

}// namespace gui
