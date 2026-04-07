#pragma once
#include "Common/OpenCV/GUI/iGraphicalElement.hpp"

#include "Common/Type/Percent.hpp"

namespace gui {

struct Image : public iGraphicalElement< cv::Mat > {
public:
    struct Config {
        cv::Size size;
        cv::Mat  defaultImage;
    };

private:
    const cv::Size configSize;
    cv::Mat        drawImage;
    cv::Mat        resizeDefaultImage;

    void drawElement(cv::Mat& image, cv::Rect busyRect) override {

        cv::Rect roi(busyRect.x, busyRect.y, drawImage.cols, drawImage.rows);
        cv::Mat  destinationROI = image(roi);
        drawImage.copyTo(destinationROI);
    }

public:
    Image(const Config& config) : configSize(config.size) {
        cv::resize(config.defaultImage, resizeDefaultImage, configSize);
        resetImage();
    }

    void setImage(const cv::Mat& image) {
        drawImage = cv::Mat();
        cv::resize(image, drawImage, configSize);
    }
    void resetImage() { drawImage = resizeDefaultImage; }

    cv::Size elementSize() override { return configSize; }
};

}// namespace gui
