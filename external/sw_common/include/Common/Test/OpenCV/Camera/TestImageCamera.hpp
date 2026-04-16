#pragma once

#include <atomic>
#include <filesystem>

#include "Common/OpenCV/Camera/iPullCamera.hpp"
#include "Common/OpenCV/CopyMat.hpp"
#include "Common/OpenCV/Image.hpp"

class TestImageCamera : public iPullCamera< CopyMat > {
public:
    struct Config {
        Angle2                angleOfView;
        std::filesystem::path filePath;
    };

private:
    std::filesystem::path filePath;
    CopyMat               image;
    std::atomic< bool >   isConnect;

public:
    TestImageCamera(const std::shared_ptr< iLogger >& logger, const Config& config)
        : iPullCamera< CopyMat >(logger, config.angleOfView), filePath(config.filePath), isConnect(false) {}

    void connect() override {
        image     = readImageInMode(filePath, cv::ImreadModes::IMREAD_COLOR);
        isConnect = true;
    };

    virtual void disconnect() override { isConnect = false; };

    virtual bool isConnection() override { return isConnect; };

protected:
    CameraFrame< CopyMat > _readFrame() override {
        if (!isConnect) {
            throw std::runtime_error("Camera is not connection");
        }
        return CameraFrame< CopyMat >{
            .timestamp   = std::chrono::steady_clock::now().time_since_epoch(),
            .image       = image,
            .angleOfView = iCamera< CopyMat >::angleOfView,
        };
    }
};