#pragma once

#include "Common/OpenCV/Camera/iCameraActiveModule.hpp"
#include "Common/OpenCV/Camera/iPushCamera.hpp"

template < typename TImage > class PushCameraActiveModule : public iCameraActiveModule< TImage > {
private:
    using typename iCameraActiveModule< TImage >::CameraState;
    using iCameraActiveModule< TImage >::cameraStateMutex;
    using iCameraActiveModule< TImage >::cameraStateCondition;
    using iCameraActiveModule< TImage >::cameraState;
    using iCameraActiveModule< TImage >::logger;

private:
    const std::string               CALLBACK_MODULE_TITLE = "CAMERA-AM";
    const std::chrono::milliseconds DELAY_CONNECT_CHECK   = 500ms;
    const std::chrono::milliseconds DELAY_BETWEEN_ERROR   = 1000ms;

    std::unique_ptr< iPushCamera< TImage > > camera;

public:
    PushCameraActiveModule(std::shared_ptr< iModuleLogger >& logger, std::unique_ptr< iPushCamera< TImage > > camera)
        : iCameraActiveModule< TImage >(logger), camera(std::move(camera)) {
        this->camera->frameCallback.subscribeToUniqueByDirect(
                [&](std::unique_ptr< CameraFrame< TImage > > frame) {
                    this->frameCallback.notify(std::move(frame));
                },
                CALLBACK_MODULE_TITLE);
    }
    ~PushCameraActiveModule() override { this->kill(); }

    bool isConnection() override { return camera->isConnection(); }

    bool isParameter(std::string_view key) const override { return camera->isParameter(key); }
    const iCamera< TImage >::Parameter getParameter(std::string_view key) const override {
        return camera->getParameter(key);
    }
    void         setParameter(std::string_view key, double value) override { camera->setParameter(key, value); }
    const Angle2 getAngleOfView() const override { return camera->getAngleOfView(); }

protected:
    void stop() override {
        cameraStateCondition.notify_all();
        camera->disconnect();
    }
    std::chrono::duration< double > action() override {
        try {
            if (camera->isConnection()) {
                switch (cameraState) {
                case CameraState::AUTOCONNECT: {
                    return DELAY_CONNECT_CHECK;// TODO: add check aps frameCallback
                } break;
                case CameraState::DISCONNECT:
                default: {
                    camera->disconnect();
                } break;
                }
            } else {
                switch (cameraState) {
                case CameraState::AUTOCONNECT: {
                    camera->connect();
                } break;
                case CameraState::DISCONNECT:
                default: {
                    std::unique_lock< std::mutex > lock(cameraStateMutex);
                    cameraStateCondition.wait(lock, [this]() {
                        return cameraState != CameraState::DISCONNECT || !this->isWork();
                    });
                } break;
                }
            }
        } catch (const std::exception& e) {
            logger->error(e.what());
            return DELAY_BETWEEN_ERROR;
        }
        return 0ms;
    }
};