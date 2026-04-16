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
        Angle2      angleOfView;
        std::string netSourceLink;
    };

private:
    std::string netSourceLink;

public:
    NetCamera(const std::shared_ptr< iLogger >& logger, const Config& config)
        : OpenCVCamera(logger, config.angleOfView), netSourceLink(config.netSourceLink) {}

    void connect() override {
        std::unique_lock< std::mutex > lock(mutex);

        if (!capture.open(netSourceLink)) {
            release();
            throw std::runtime_error("Could not open camera network link: " + netSourceLink);
        }

        cv::Mat temp_image;
        if (!capture.read(temp_image)) {
            release();
            throw std::runtime_error("Camera read timeout after init");
        }

        isConnect = true;
    };
};