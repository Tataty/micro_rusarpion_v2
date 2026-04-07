#pragma once

#include "Common/Module/Callback/MultiCallback.hpp"
#include "Common/OpenCV/Camera/iCamera.hpp"
#include "Common/Utils/TimeDelayMeter.hpp"

template < typename TImage > class iPushCamera : public iCamera< TImage > {
protected:
    void frameCallbackNotify(TImage&& image) {
        std::unique_ptr< CameraFrame< TImage > > frame = std::make_unique< CameraFrame< TImage > >();

        frame->timestamp   = std::chrono::steady_clock::now().time_since_epoch();
        frame->image       = std::move(image);
        frame->angleOfView = iCamera< TImage >::parameters.angleOfView;

        frameCallback.notify(std::move(frame));

        this->logFrameIntervals();
    }

public:
    MultiCallback< CameraFrame< TImage > > frameCallback;

    iPushCamera(const std::shared_ptr< iLogger >& logger, const CameraParameters& parameters)
        : iCamera< TImage >(logger, parameters), frameCallback(logger) {}
};