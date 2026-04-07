#pragma once

#include <memory>
#include "Common/Logger/iLogger.hpp"
#include "Common/Utils/TimeDelayMeter.hpp"

class CallbackPassLogger {
public:
    static constexpr double LOG_DELAY = 1; /* in seconds */

private:
    std::shared_ptr< iLogger > logger;
    size_t                     numberOfPasses = 0;
    TimeDelayMeter             timeDelayMeter;
    const std::string          linkTitle;

public:
    CallbackPassLogger(const std::shared_ptr< iLogger >& logger, const std::string& linkTitle)
        : logger(logger), linkTitle(linkTitle) {}

    void log() {
        if (timeDelayMeter.getDelaySeconds() > LOG_DELAY) {

            if (numberOfPasses > 0)
                logger->warning("{} Callback passes: {}", linkTitle, numberOfPasses);

            timeDelayMeter.resetTimer();
            numberOfPasses = 0;
        }
    }

    void pass() { numberOfPasses++; }
};