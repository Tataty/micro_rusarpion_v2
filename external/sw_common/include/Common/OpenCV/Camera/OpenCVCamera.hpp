#pragma once

#include <atomic>
#include "Common/OpenCV/Camera/iPullCamera.hpp"
#include "Common/OpenCV/CopyMat.hpp"

class OpenCVCamera : public iPullCamera< CopyMat > {
protected:
    cv::VideoCapture    capture;
    std::atomic< bool > isConnect;

public:
    OpenCVCamera(const std::shared_ptr< iLogger >& logger, const Angle2& angleOfView)
        : iPullCamera< CopyMat >(logger, angleOfView), isConnect(false) {}

    virtual void disconnect() override {
        std::unique_lock< std::mutex > lock(mutex);
        release();
    };

    virtual bool isConnection() override { return isConnect; };

protected:
    void release() {
        capture.release();
        isConnect = false;
    }

    virtual CameraFrame< CopyMat > _readFrame() override {

        if (!capture.isOpened()) {
            release();
            throw std::runtime_error("Camera is not connection");
        }

        cv::Mat outputImage;
        if (!capture.read(outputImage)) {
            release();
            throw std::runtime_error("Camera read timeout");
        }

        if (outputImage.empty()) {
            release();
            throw std::runtime_error("Camera image is empty");
        }

        return CameraFrame< CopyMat >{
            .timestamp   = std::chrono::steady_clock::now().time_since_epoch(),
            .image       = outputImage,
            .angleOfView = iCamera< CopyMat >::angleOfView,
        };
    }
};