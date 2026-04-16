#pragma once

#include <chrono>
#include <opencv2/opencv.hpp>
#include "Common/Type/Angle.hpp"

template < typename TImage > struct CameraFrame {
    std::chrono::nanoseconds timestamp;
    TImage                   image;
    Angle2                   angleOfView;
};

struct CameraFrameData {
    std::chrono::nanoseconds timestamp;
    Angle2                   angleOfView;
    cv::Size                 resolution;

    double pitchAngleInPixels(const Angle& pitchAngle) const {
        return pitchAngle / angleOfView.pitch * resolution.height;
    }
    double yawAngleInPixels(const Angle& yawAngle) const { return yawAngle / angleOfView.yaw * resolution.width; }

    cv::Size angleToSize(const Angle2& angles) const {
        cv::Size size;

        size.width  = abs(yawAngleInPixels(angles.yaw));
        size.height = abs(pitchAngleInPixels(angles.pitch));

        return size;
    }

    cv::Point angleToPoint(const Angle2& angles) const {
        cv::Point point;

        point.x = yawAngleInPixels(angles.yaw);
        point.y = pitchAngleInPixels(angles.pitch);

        convertToCCS(point);
        return point;
    }

    Angle2 pointToAngle(const cv::Point& point) const {
        Angle2 angles;

        angles.yaw   = angleOfView.yaw * point.x / resolution.width;
        angles.pitch = angleOfView.pitch * point.y / resolution.height;

        convertToCCS(angles);
        return angles;
    }

    double calculateOpticalDistance(const cv::Size2d& realSize, const cv::Rect& bBox) const {
        if (realSize.width > 0)
            return (realSize.width * resolution.width) / (2 * tan(angleOfView.yaw.getRadians() / 2) * bBox.width);
        else
            return (realSize.height * resolution.height) / (2 * tan(angleOfView.pitch.getRadians() / 2) * bBox.height);
    }

    static inline void convertToCCS(Angle2& angles) {
        angles.yaw   = angles.yaw;
        angles.pitch = -angles.pitch;
    }
    static inline void convertToCCS(cv::Point& offset) {
        offset.x = offset.x;
        offset.y = -offset.y;
    }
};

#include "Common/OpenCV/CopyMat.hpp"

CameraFrameData extractCameraFrameData(const CameraFrame< CopyMat >& cameraFrame);