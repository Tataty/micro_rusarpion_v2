#pragma once

#include <functional>
#include <memory>
#include "Common/Logger/iLogger.hpp"

template < typename TDataPtr > class iCallbackChannel {
public:
    using TCallbackFunction = std::function< void(TDataPtr) >;

    struct Config {
        std::string linkTitle;
    };

protected:
    const Config                     config;
    const std::shared_ptr< iLogger > logger;
    const TCallbackFunction          callbackFunction;

    void sendDataToCallbackFunction(TDataPtr& data) {
        try {
            callbackFunction(std::move(data));
        } catch (const std::exception& e) {
            logger->error("{} {}", config.linkTitle, e.what());
        }
    }

public:
    virtual void push(TDataPtr data) = 0;

    iCallbackChannel(const std::shared_ptr< iLogger >& logger,
                     const Config&                     config,
                     const TCallbackFunction&          callbackFunction)
        : config(config), logger(logger), callbackFunction(callbackFunction) {}
    virtual ~iCallbackChannel() = default;
};
