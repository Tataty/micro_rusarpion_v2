#pragma once

#include "Common/OpenCV/Camera/iPullCamera.hpp"

class OpenCVCamera : public iPullCamera< cv::Mat > {
protected:
    cv::VideoCapture capture;

public:
    OpenCVCamera(const std::shared_ptr< iLogger >& logger, const CameraParameters& parameters)
        : iPullCamera< cv::Mat >(logger, parameters) {}

    virtual void disconnect() override {
        std::unique_lock< std::mutex > lock(mutex);
        capture.release();
    };

    virtual bool isConnection() override {
        std::unique_lock< std::mutex > lock(mutex);
        return capture.isOpened();
    };

protected:
    virtual cv::Mat readImage() override {

        if (!capture.isOpened()) {
            capture.release();
            throw std::runtime_error("Camera is not connection");
        }

        cv::Mat outputImage;
        if (!capture.read(outputImage)) {
            capture.release();
            throw std::runtime_error("Camera read timeout");
        }

        if (outputImage.empty()) {
            capture.release();
            throw std::runtime_error("Camera image is empty");
        }
        return outputImage;
    }
};