#pragma once

#include "Common/OpenCV/Camera/iPullCamera.hpp"
#include "Common/OpenCV/CopyMat.hpp"
#include "Common/OpenCV/Image.hpp"

class FlipCameraDecorator : public iPullCamera< CopyMat > {
public:
    static constexpr double FLIP_NONE = 0.d;
    static constexpr double FLIP_X    = 1.d;
    static constexpr double FLIP_Y    = 2.d;
    static constexpr double FLIP_XY   = 3.d;

private:
    std::unique_ptr< iPullCamera< CopyMat > > camera;

    Parameter flipParameter;

public:
    FlipCameraDecorator(std::unique_ptr< iPullCamera< CopyMat > > camera)
        : iPullCamera< CopyMat >(camera->getLogger(), camera->getAngleOfView()), camera(std::move(camera)) {
        /* FLIP PARAMETER */
        flipParameter.value    = 0.d;
        flipParameter.minValue = 0.d;
        flipParameter.maxValue = 3.d;
    }

    void connect() override { camera->connect(); }
    void disconnect() override { camera->disconnect(); }
    bool isConnection() override { return camera->isConnection(); }

    bool isParameter(std::string_view key) const override {
        if (key == "flip") {
            return true;
        }
        return camera->isParameter(key);
    }
    const Parameter getParameter(std::string_view key) const override {
        if (key == "flip") {
            return flipParameter;
        }
        return camera->getParameter(key);
    }
    void setParameter(std::string_view key, double value) override {
        if (key == "flip") {

            flipParameter.setValue(value);
            return;
        }
        camera->setParameter(key, value);
    }

protected:
    CameraFrame< CopyMat > _readFrame() override {
        CameraFrame< CopyMat > cameraFrame = camera->readFrame();

        if (flipParameter.value == FLIP_XY) {
            cv::flip(cameraFrame.image.mat, cameraFrame.image.mat, -1);
        } else if (flipParameter.value == FLIP_X) {
            cv::flip(cameraFrame.image.mat, cameraFrame.image.mat, 1);
        } else if (flipParameter.value == FLIP_Y) {
            cv::flip(cameraFrame.image.mat, cameraFrame.image.mat, 0);
        }

        return cameraFrame;
    };
};