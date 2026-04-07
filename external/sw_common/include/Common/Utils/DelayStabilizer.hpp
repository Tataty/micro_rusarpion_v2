#pragma once
#include <stdexcept>
#include "Common/Utils/TimeDelayMeter.hpp"

class DelayStabilizer {
private:
    TimeDelayMeter                  timeDelayMeter;
    std::chrono::duration< double > stableDelay;
    std::chrono::duration< double > lastDelay;

public:
    DelayStabilizer(std::chrono::duration< double > stableDelay) : stableDelay(stableDelay) { resetTimer(); }

    std::chrono::duration< double > getDelay() {
        std::chrono::duration< double > currentDelay = timeDelayMeter.getDelay() - lastDelay;

        timeDelayMeter.resetTimer();

        auto outputDelay = stableDelay - currentDelay;
        lastDelay        = outputDelay;

        return outputDelay;
    }

    void resetTimer() {
        timeDelayMeter.resetTimer();
        lastDelay = std::chrono::duration< double >::zero();
    }
};