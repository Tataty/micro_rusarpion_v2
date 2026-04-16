#pragma once

#include <chrono>
#include <cmath>
#include "Common/Utils/TimeDelayMeter.hpp"

/**
 * @brief Class for creating a periodic logic signal (square wave).
 * Uses TimeDelayMeter to track the phase of the signal.
 */
class Oscillator {
private:
    TimeDelayMeter                  timer;
    std::chrono::duration< double > period;

public:
    /**
     * @brief Construct a new Oscillator object.
     * @param period The duration of one full cycle (High + Low).
     */
    explicit Oscillator(std::chrono::duration< double > period) : period(period) { timer.resetTimer(); }

    /**
     * @brief Sets the full cycle period.
     * @param p New duration of the cycle.
     */
    void setPeriod(std::chrono::duration< double > p) { period = p; }

    /**
     * @brief Returns the current period duration.
     */
    std::chrono::duration< double > getPeriod() const { return period; }

    /**
     * @brief Returns true (High) if the current phase is in the first half of the period.
     * Returns false (Low) if it is in the second half or if period is zero.
     */
    bool isActive() {
        if (period.count() <= 0.0) {
            return false;
        }

        const double elapsedSeconds = timer.getDelay().count();
        const double periodSeconds  = period.count();

        const double currentPhase = std::fmod(elapsedSeconds, periodSeconds);

        return currentPhase < (periodSeconds / 2.0);
    }

    /**
     * @brief Resets the oscillator phase to the beginning (sets state to High).
     */
    void restart() { timer.resetTimer(); }
};