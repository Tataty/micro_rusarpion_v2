#pragma once
#include <fmt/core.h>
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Camera/CameraFrame.hpp"
#include "Common/ToString.hpp"

#include "Common/Logger/iLogger.hpp"
#include "Common/Utils/TimeDelayMeter.hpp"

template < typename TImage > class iCamera {
private:
    static constexpr std::chrono::duration< double > FRAME_INTERVAL_DELAY = std::chrono::seconds(1);

    TimeDelayMeter        logFrameIntervalMeter;
    TimeDelayMeter        frameIntervalMeter;
    std::vector< double > listOfFrameInterval;

protected:
    void logFrameIntervals() {
        listOfFrameInterval.push_back(frameIntervalMeter.getDelaySeconds());
        frameIntervalMeter.resetTimer();

        if (logFrameIntervalMeter.getDelay() > FRAME_INTERVAL_DELAY) {
            logFrameIntervalMeter.resetTimer();

            this->logger->trace("Frame intervals: {}", toString(listOfFrameInterval));

            listOfFrameInterval.clear();
        }
    }

protected:
    const std::shared_ptr< iLogger > logger;

    Angle2 angleOfView;

public:
    iCamera(const std::shared_ptr< iLogger >& logger, const Angle2& angleOfView)
        : logger(logger), angleOfView(angleOfView) {}
    virtual ~iCamera() = default;

    virtual void connect()      = 0;
    virtual void disconnect()   = 0;
    virtual bool isConnection() = 0;

    const std::shared_ptr< iLogger > getLogger() const { return logger; }
    const Angle2                     getAngleOfView() const { return angleOfView; }

    /* PARAMETRIZATION */
    struct Parameter {
        double value;

        double minValue;
        double maxValue;

        void   setValue(double newValue) { value = std::clamp(newValue, minValue, maxValue); }
        double getClamp(double value) { return std::clamp(value, minValue, maxValue); }
    };

    virtual bool            isParameter(std::string_view key) const { return false; }
    virtual const Parameter getParameter(std::string_view key) const {
        throw std::runtime_error(fmt::format("Parameter does not exist with key: {}", key));
    }
    virtual void setParameter(std::string_view key, double value) {
        throw std::runtime_error(fmt::format("Parameter does not exist with key: {}", key));
    }
};