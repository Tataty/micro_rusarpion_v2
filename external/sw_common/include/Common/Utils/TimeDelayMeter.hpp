#pragma once

#include <chrono>

class TimeDelayMeter {
private:
    std::chrono::steady_clock::time_point lastTime;

    bool isResetTimer = false;

public:
    TimeDelayMeter() { resetTimer(); }

    std::chrono::duration< double > getDelay() { return std::chrono::steady_clock::now() - lastTime; }
    double                          getDelaySeconds() { return getDelay().count(); }

    void resetTimer() { lastTime = std::chrono::steady_clock::now(); }
    void resetTimerOnce() {
        if (isResetTimer)
            return;

        isResetTimer = true;
        lastTime     = std::chrono::steady_clock::now();
    }
    void enableResetTimer() { isResetTimer = false; }
};