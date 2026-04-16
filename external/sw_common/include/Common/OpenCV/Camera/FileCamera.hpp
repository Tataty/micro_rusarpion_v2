#pragma once

#include <filesystem>
#include <thread>

#include "Common/OpenCV/Camera/OpenCVCamera.hpp"

class FileCamera : public OpenCVCamera {
public:
    struct Config {
        Angle2                angleOfView;
        std::filesystem::path filePath;
    };

private:
    std::filesystem::path     filePath;
    std::chrono::milliseconds frameDelay;

public:
    FileCamera(const std::shared_ptr< iLogger >& logger, const Config& config)
        : OpenCVCamera(logger, config.angleOfView), filePath(config.filePath) {}

    void connect() override {
        std::unique_lock< std::mutex > lock(mutex);

        if (!capture.open(filePath)) {
            capture.release();
            throw std::runtime_error("Could not open camera from file: " + filePath.string());
        }

        frameDelay = std::chrono::milliseconds(static_cast< int >(1000 / capture.get(cv::CAP_PROP_FPS)));
        isConnect  = true;
    };

protected:
    CameraFrame< CopyMat > _readFrame() override {
        std::this_thread::sleep_for(frameDelay);

        if (!capture.isOpened()) {
            release();
            throw std::runtime_error("Camera is not connection");
        }

        cv::Mat readMat;
        if (!capture.read(readMat) || readMat.empty()) {
            capture.set(cv::CAP_PROP_POS_FRAMES, 0);

            if (!capture.read(readMat) || readMat.empty()) {
                release();
                throw std::runtime_error("Failed to restart video loop");
            }
        }

        return CameraFrame< CopyMat >{
            .timestamp   = std::chrono::steady_clock::now().time_since_epoch(),
            .image       = readMat,
            .angleOfView = iCamera< CopyMat >::angleOfView,
        };
    }
};