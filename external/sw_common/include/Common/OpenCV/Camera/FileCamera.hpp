#pragma once
#include <filesystem>
#include <thread>

#include "Common/OpenCV/Camera/OpenCVCamera.hpp"

class FileCamera : public OpenCVCamera {
public:
    struct Config {
        iCamera::Config       iCameraConfig;
        std::filesystem::path filePath;
    };

private:
    std::filesystem::path     filePath;
    std::chrono::milliseconds frameDelay;

public:
    FileCamera(const Config& config) : OpenCVCamera(config.iCameraConfig), filePath(config.filePath) {}

    void connect() override {
        std::unique_lock< std::mutex > lock(mutex);

        if (!capture.open(filePath)) {
            capture.release();
            throw std::runtime_error("Could not open camera from file: " + filePath.string());
        }

        frameDelay = std::chrono::milliseconds(static_cast< int >(1000 / capture.get(cv::CAP_PROP_FPS)));
    };

protected:
    cv::Mat readMat() override {
        std::this_thread::sleep_for(frameDelay);

        cv::Mat outputImage = OpenCVCamera::readMat();

        return outputImage;
    }
};