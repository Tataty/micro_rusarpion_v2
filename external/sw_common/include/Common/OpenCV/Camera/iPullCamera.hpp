#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Camera/iCamera.hpp"

template < typename TImage > class iPullCamera : public iCamera< TImage > {
protected:
    std::mutex mutex;

public:
    iPullCamera(const std::shared_ptr< iLogger >& logger, const Angle2& angleOfView)
        : iCamera< TImage >(logger, angleOfView) {}
    virtual ~iPullCamera() = default;

    CameraFrame< TImage > readFrame() {
        std::unique_lock< std::mutex > lock(mutex);

        this->logFrameIntervals();

        return _readFrame();
    }

protected:
    virtual CameraFrame< TImage > _readFrame() = 0;
};