#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Camera/iCamera.hpp"

template < typename TImage > class iPullCamera : public iCamera< TImage > {
protected:
    virtual TImage readImage() = 0;

    std::mutex mutex;

public:
    iPullCamera(const std::shared_ptr< iLogger >& logger, const CameraParameters& parameters)
        : iCamera< TImage >(logger, parameters) {}
    virtual ~iPullCamera() = default;

    CameraFrame< TImage > readFrame() {
        std::unique_lock< std::mutex > lock(mutex);

        CameraFrame< TImage > outputFrame = {
            .timestamp   = std::chrono::steady_clock::now().time_since_epoch(),
            .image       = readImage(),
            .angleOfView = iCamera< TImage >::parameters.angleOfView,
        };

        this->logFrameIntervals();

        return outputFrame;
    }
};