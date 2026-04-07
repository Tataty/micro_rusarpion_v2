#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <stdexcept>
#include <thread>

#include "Common/Module/Callback/MultiCallback.hpp"
#include "Common/Module/iActiveModule.hpp"
#include "Common/OpenCV/Camera/CameraFrame.hpp"

template < typename TImage > class iCameraActiveModule : public iActiveModule {
protected:
    const std::chrono::milliseconds DELAY_BETWEEN_ERROR = 1000ms;

    enum class CameraState { DISCONNECT, AUTOCONNECT };

    constexpr std::string_view toString(CameraState state) {
        switch (state) {
        case CameraState::DISCONNECT:
            return "DISCONNECT";
        case CameraState::AUTOCONNECT:
            return "AUTOCONNECT";
        default:
            return "UNKNOWN";
        }
    }

    std::mutex              cameraStateMutex;
    std::condition_variable cameraStateCondition;
    CameraState             cameraState = CameraState::DISCONNECT;

public:
    MultiCallback< CameraFrame< TImage > > frameCallback;

    iCameraActiveModule(std::shared_ptr< iModuleLogger >& logger)
        : iActiveModule(logger), frameCallback(this->logger) {}

    void connect() { setCameraState(CameraState::AUTOCONNECT); }
    void disconnect() { setCameraState(CameraState::DISCONNECT); }

private:
    void setCameraState(CameraState newCameraState) {
        std::unique_lock< std::mutex > lock(cameraStateMutex);

        cameraState = newCameraState;
        cameraStateCondition.notify_all();

        logger->info("Set camera state: {}", toString(newCameraState));
    }
};