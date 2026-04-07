#pragma once

#include <stdint.h>

#include "Common/Type/Percent.hpp"
#include "Common/Type/State.hpp"

namespace sbus {

static constexpr uint16_t CHANNEL_MAX_VAL = 1900;
static constexpr uint16_t CHANNEL_MIN_VAL = 1100;
static constexpr uint16_t CHANNEL_CENTER  = (CHANNEL_MAX_VAL + CHANNEL_MIN_VAL) / 2;

static constexpr Percent PERCENT_LOW_STATE    = PERCENT_QUARTER;
static constexpr Percent PERCENT_HIGH_STATE   = PERCENT_QUARTER;
static constexpr Percent PERCENT_MEDIUM_STATE = PERCENT_HUNDRED - (PERCENT_LOW_STATE + PERCENT_HIGH_STATE);

Percent     ChannelToPercent(uint16_t channel);
DoubleState ChannelToDoubleState(uint16_t channel);
TripleState ChannelToThirdState(uint16_t channel);

class ChannelXState {
private:
    size_t numberOfStates;

public:
    ChannelXState(size_t numberOfStates) : numberOfStates(numberOfStates) {}

    size_t getState(uint16_t channel) {
        Percent percent = ChannelToPercent(channel);
        return percent.getNormalize() * numberOfStates;
    }
};

};// namespace sbus