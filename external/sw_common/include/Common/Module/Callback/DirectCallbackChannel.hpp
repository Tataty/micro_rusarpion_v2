#pragma once

#include "Common/Module/Callback/iCallbackChannel.hpp"

template < typename TDataPtr > class DirectCallbackChannel : public iCallbackChannel< TDataPtr > {
public:
    using typename iCallbackChannel< TDataPtr >::TCallbackFunction;
    using typename iCallbackChannel< TDataPtr >::Config;

public:
    void push(TDataPtr data) override { this->sendDataToCallbackFunction(data); }

    DirectCallbackChannel(const std::shared_ptr< iLogger >& logger,
                          const Config&                     config,
                          const TCallbackFunction&          callbackFunction)
        : iCallbackChannel< TDataPtr >(logger, config, callbackFunction) {}
};