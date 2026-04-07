#pragma once
#include <chrono>

#include "Common/OpenCV/CopyMat.hpp"
#include "Common/Type/Angle.hpp"

#include "Common/OpenCV/Common.hpp"

template < typename TImage > struct CameraFrame {
    std::chrono::nanoseconds timestamp;
    TImage                   image;
    Angle2                   angleOfView;
};

/* TODO:

class iImage {
public:
    virtual size_t getHeight() = 0;
    virtual size_t getWidth()  = 0;
}

struct CameraFrame {
    std::chrono::nanoseconds  timestamp;
    std::unique_ptr< iImage > image;
    Angle2                    angleOfView;

    double pitchAngleInPixels(const Angle& pitchAngle) const {
        return pitchAngle / angleOfView.pitch * image->getHeight();
    }
    double yawAngleInPixels(const Angle& yawAngle) const { return yawAngle / angleOfView.yaw * image->getWidth(); }
};

*/