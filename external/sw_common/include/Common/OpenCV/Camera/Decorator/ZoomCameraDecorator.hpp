#pragma once

#include "Common/OpenCV/Camera/iPullCamera.hpp"
#include "Common/OpenCV/CopyMat.hpp"
#include "Common/OpenCV/Image.hpp"

class ZoomCameraDecorator : public iPullCamera< CopyMat > {
private:
    std::unique_ptr< iPullCamera< CopyMat > > camera;

    Parameter zoomXParameter;

public:
    ZoomCameraDecorator(std::unique_ptr< iPullCamera< CopyMat > > camera)
        : iPullCamera< CopyMat >(camera->getLogger(), camera->getAngleOfView()), camera(std::move(camera)) {
        /* ZOOMX PARAMETER */
        zoomXParameter.value    = 1.d;
        zoomXParameter.minValue = 1.d;
        zoomXParameter.maxValue = 4.d;
    }

    void connect() override { camera->connect(); }
    void disconnect() override { camera->disconnect(); }
    bool isConnection() override { return camera->isConnection(); }

    bool isParameter(std::string_view key) const override {
        if (key == "zoomX") {
            return true;
        }
        return camera->isParameter(key);
    }
    const Parameter getParameter(std::string_view key) const override {
        if (key == "zoomX") {
            return zoomXParameter;
        }
        return camera->getParameter(key);
    }
    void setParameter(std::string_view key, double value) override {
        if (key == "zoomX") {

            zoomXParameter.setValue(value);
            angleOfView = camera->getAngleOfView() / zoomXParameter.value;
            return;
        }
        camera->setParameter(key, value);
    }

protected:
    CameraFrame< CopyMat > _readFrame() override {
        CameraFrame< CopyMat > cameraFrame = camera->readFrame();

        cameraFrame.image.mat   = zoomImage(cameraFrame.image.mat, zoomXParameter.value);
        cameraFrame.angleOfView = angleOfView;

        return cameraFrame;
    };
};