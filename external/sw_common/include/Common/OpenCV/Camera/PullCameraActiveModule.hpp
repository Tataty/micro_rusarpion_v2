#pragma once

#include "Common/OpenCV/Camera/iCameraActiveModule.hpp"
#include "Common/OpenCV/Camera/iPullCamera.hpp"

template < typename TImage > class PullCameraActiveModule : public iCameraActiveModule< TImage > {
private:
    using typename iCameraActiveModule< TImage >::CameraState;
    using iCameraActiveModule< TImage >::cameraStateMutex;
    using iCameraActiveModule< TImage >::cameraStateCondition;
    using iCameraActiveModule< TImage >::cameraState;
    using iCameraActiveModule< TImage >::logger;

private:
    const std::chrono::milliseconds DELAY_BETWEEN_FRAMES = 0ms;
    const std::chrono::milliseconds DELAY_BETWEEN_ERROR  = 1000ms;

    std::unique_ptr< iPullCamera< TImage > > camera;

public:
    PullCameraActiveModule(std::shared_ptr< iModuleLogger >& logger, std::unique_ptr< iPullCamera< TImage > > camera)
        : iCameraActiveModule< TImage >(logger), camera(std::move(camera)) {}
    ~PullCameraActiveModule() override { this->kill(); }

    bool isConnection() { return camera->isConnection(); }

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

                    this->frameCallback.notify(std::make_unique< CameraFrame< TImage > >(camera->readFrame()));

                    return DELAY_BETWEEN_FRAMES;
                } break;
                case CameraState::DISCONNECT:
                default:
                    camera->disconnect();
                    break;
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