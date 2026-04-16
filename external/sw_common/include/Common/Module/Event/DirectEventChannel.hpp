#pragma once

#include "Common/Module/Event/iEventChannel.hpp"

class DirectEventChannel : public iEventChannel {
public:
    void push() override { this->pushEventFunction(); }

    DirectEventChannel(const std::shared_ptr< iLogger >& logger,
                       const Config&                     config,
                       const TEventFunction&             eventFunction)
        : iEventChannel(logger, config, eventFunction) {}
};