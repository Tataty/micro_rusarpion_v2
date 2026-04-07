/**
 * @file USBCamera.hpp
 * @author Ruslan L.
 * @brief USB camera module
 *
 * @copyright Copyright (c) 2025
**/

#pragma once

#include <fmt/core.h>
#include <optional>

#include "Common/OpenCV/Camera/CameraFormat.hpp"
#include "Common/OpenCV/Camera/CameraRequirement.hpp"
#include "Common/OpenCV/Camera/OpenCVCamera.hpp"

// TODO: go to libcamerasrc
/* USB camera, although opencv has the ability to capture video from anything, the ability to do this is reduced to ensure a single responsibility */
class USBCamera : public OpenCVCamera {
public:
    static constexpr std::array< CameraFormat, 2 > SUPPORT_CAMERA_FORMAT{ CameraFormat::MJPG, CameraFormat::YUYV };

    struct Config {
        CameraParameters                   cameraParameters;
        std::string                        device;
        std::optional< CameraRequirement > requirement;
    };

private:
    std::string pipeline;

    static inline bool isSupportFormat(const CameraFormat& f) {
        for (auto s : SUPPORT_CAMERA_FORMAT)
            if (s == f)
                return true;
        return false;
    }

    const std::string PIPELINE_OUTPUT =
            "! videoconvert n-threads=1 qos=true ! video/x-raw,format=BGR ! queue max-size-buffers=1 leaky=downstream ! appsink drop=true max-buffers=1 sync=false";

    inline std::string buildPipelineDefault(const std::string& device) {
        return fmt::format("v4l2src device={} ! video/x-raw {}", device, PIPELINE_OUTPUT);
    }

    inline std::string buildPipelineMJPG(const std::string& device, const CameraRequirement& cameraRequirement) {
        return fmt::format("v4l2src device={} ! image/jpeg, width={}, height={}, framerate={}/1 ! jpegdec {}",
                           device,
                           cameraRequirement.resolution.width,
                           cameraRequirement.resolution.height,
                           cameraRequirement.fps,
                           PIPELINE_OUTPUT);
    }
    inline std::string buildPipelineYUYV(const std::string& device, const CameraRequirement& cameraRequirement) {
        return fmt::format("v4l2src device={} ! video/x-raw,format=YUY2,width={},height={},framerate={}/1 {}",
                           device,
                           cameraRequirement.resolution.width,
                           cameraRequirement.resolution.height,
                           cameraRequirement.fps,
                           PIPELINE_OUTPUT);
    }

    std::string buildPipelineByCameraRequirement(const std::string& device, CameraRequirement cameraRequirement) {
        switch (cameraRequirement.format.value()) {
        case CameraFormat::MJPG_RAW:
            return buildPipelineMJPG(device, cameraRequirement);
        case CameraFormat::YUYV_RAW:
            return buildPipelineYUYV(device, cameraRequirement);
        default:
            throw std::runtime_error("Unsupported format");
        }
    }

public:
    USBCamera(const std::shared_ptr< iLogger >& logger, const Config& config)
        : OpenCVCamera(logger, config.cameraParameters) {
        if (config.requirement.has_value()) {
            pipeline = buildPipelineByCameraRequirement(config.device, config.requirement.value());
        } else {
            pipeline = buildPipelineDefault(config.device);
        }
    }

    void connect() override {
        std::unique_lock< std::mutex > lock(mutex);

        if (!capture.open(pipeline, cv::CAP_GSTREAMER)) {
            capture.release();
            throw std::runtime_error("Could not open camera pipeline: " + pipeline);
        }

        cv::Mat tempFrame;
        if (!capture.read(tempFrame)) {
            capture.release();
            throw std::runtime_error("Camera read timeout after init");
        }
    };
};