/**
 * @file NetCamera.hpp
 * @author Ruslan L.
 * @brief Camera module for network abstract
 *
 * @copyright Copyright (c) 2025
**/

#pragma once

#include "Common/OpenCV/Camera/OpenCVCamera.hpp"

class NetCamera : public OpenCVCamera {
public:
    struct Config {
        iCamera::Config iCameraConfig;
        std::string     netSourceLink;
    };

private:
    std::string netSourceLink;

public:
    NetCamera(const Config& config) : OpenCVCamera(config.iCameraConfig), netSourceLink(config.netSourceLink) {}

    void connect() override {
        std::unique_lock< std::mutex > lock(mutex);

        if (!capture.open(netSourceLink)) {
            capture.release();
            throw std::runtime_error("Could not open camera network link: " + netSourceLink);
        }

        cv::Mat temp_image;
        if (!capture.read(temp_image)) {
            capture.release();
            throw std::runtime_error("Camera read timeout after init");
        }
    };
};