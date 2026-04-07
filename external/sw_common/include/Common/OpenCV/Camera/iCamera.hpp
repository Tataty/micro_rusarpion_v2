#pragma once
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/Camera/CameraFrame.hpp"
#include "Common/ToString.hpp"

struct CameraParameters {
    Angle2 angleOfView;

    auto operator<=>(const CameraParameters&) const = default;
};

template < typename TImage > class iCamera {
protected:
    static constexpr std::chrono::duration< double > FRAME_INTERVAL_DELAY = std::chrono::seconds(1);

    TimeDelayMeter        logFrameIntervalMeter;
    TimeDelayMeter        frameIntervalMeter;
    std::vector< double > listOfFrameInterval;

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

    CameraParameters parameters;

public:
    iCamera(const std::shared_ptr< iLogger >& logger, const CameraParameters& parameters)
        : logger(logger), parameters(parameters) {}
    virtual ~iCamera() = default;

    virtual void connect()      = 0;
    virtual void disconnect()   = 0;
    virtual bool isConnection() = 0;

    const std::shared_ptr< iLogger > getLogger() const { return logger; }
    const CameraParameters           getParameters() const { return parameters; }

    // TODO: virtual void setZoom(Percent zoomRatio) = 0;
    // TODO: virtual std::string getParameter(std::string key) = 0;
    // TODO: virtual std::map< std::string, std::string > getListParameters() = 0;
    // TODO: virtual void setListParameters(std::map< std::string, std::string >) = 0;
    // TODO: virtual void setParameter(std::string, std::string) = 0;
};