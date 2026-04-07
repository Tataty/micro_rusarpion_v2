#pragma once

#include "Common/OpenCV/Camera/PullCameraActiveModule.hpp"
#include "Common/OpenCV/Camera/PushCameraActiveModule.hpp"

template < typename TImage >
std::unique_ptr< iCameraActiveModule< TImage > > createCameraActiveModule(std::shared_ptr< iModuleLogger >&    logger,
                                                                          std::unique_ptr< iCamera< TImage > > camera) {

    if (auto pullCamera = dynamic_cast< iPullCamera< TImage >* >(camera.get())) {
        camera.release();
        return std::make_unique< PullCameraActiveModule< TImage > >(
                logger, std::unique_ptr< iPullCamera< TImage > >(pullCamera));
    }

    if (auto pushCamera = dynamic_cast< iPushCamera< TImage >* >(camera.get())) {
        camera.release();
        return std::make_unique< PushCameraActiveModule< TImage > >(
                logger, std::unique_ptr< iPushCamera< TImage > >(pushCamera));
    }

    throw std::runtime_error("Unknown camera type for createCameraActiveModule");
}
