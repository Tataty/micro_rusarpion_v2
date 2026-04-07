#include "Common/Communication/SBUS.hpp"

namespace sbus {

Percent ChannelToPercent(uint16_t channel) {
    return Percent::fromRangeValue(channel, CHANNEL_MIN_VAL, CHANNEL_MAX_VAL);
}

DoubleState ChannelToDoubleState(uint16_t channel) {
    Percent percent = ChannelToPercent(channel);

    if (percent < PERCENT_HALF) {
        return DoubleState::LOW;
    }
    return DoubleState::HIGH;
}

TripleState ChannelToThirdState(uint16_t channel) {
    Percent percent = ChannelToPercent(channel);

    if (percent < PERCENT_LOW_STATE) {
        return TripleState::LOW;
    }
    if (percent < PERCENT_LOW_STATE + PERCENT_MEDIUM_STATE) {
        return TripleState::MEDIUM;
    }
    return TripleState::HIGH;
}

};// namespace sbus