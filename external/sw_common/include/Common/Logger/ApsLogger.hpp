/**
 * @file ApsLogger.hpp
 * @author Ruslan L.
 * @brief Extra class for counting APS on a second interval
 *
 * @copyright Copyright (c) 2025
 **/

#pragma once

#include "Common/Logger/iLogger.hpp"
#include "Common/Utils/TimeDelayMeter.hpp"

/* Class for counting and logging aps on second interval */
class ApsLogger {
public:
    static constexpr double LOG_DELAY = 1; /* in seconds */

private:
    std::shared_ptr< iLogger > logger;
    size_t                     actionCount = 0;
    TimeDelayMeter             timeDelayMeter;
    double                     logDelaySeconds;

public:
    ApsLogger(const std::shared_ptr< iLogger >& logger, double logDelaySeconds = LOG_DELAY)
        : logger(logger), logDelaySeconds(logDelaySeconds) {}

    /* Count and Log APS every second */
    void logApsOnceWithDelay() {

        actionCount++;
        double elapsedSeconds = timeDelayMeter.getDelaySeconds();

        if (elapsedSeconds > logDelaySeconds) {
            double aps = actionCount / elapsedSeconds;

            logger->debug("APS: {:.2}", aps);

            actionCount = 0;
            timeDelayMeter.resetTimer();
        }
    }
};