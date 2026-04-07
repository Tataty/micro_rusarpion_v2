#pragma once
#include <filesystem>
#include <opencv2/opencv.hpp>

extern std::filesystem::path IMAGES_FILE(const std::string& x);

class ImageReader {
private:
    static cv::Mat readImageInMode(const std::string& fileName, cv::ImreadModes imreadMode) {
        cv::Mat image = cv::imread(IMAGES_FILE(fileName), imreadMode);

        if (image.empty())
            throw std::runtime_error("ImageReader: Image file not found " + fileName);

        return image;
    }

public:
    static cv::Mat readImage(const std::string& fileName) { return readImageInMode(fileName, cv::IMREAD_UNCHANGED); }
    static cv::Mat readMask(const std::string& fileName) { return readImageInMode(fileName, cv::IMREAD_GRAYSCALE); }
};