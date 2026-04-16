#pragma once

#include <functional>
#include <memory>
#include "Common/Logger/iLogger.hpp"

class iEventChannel {
public:
    using TEventFunction = std::function< void() >;

    struct Config {
        std::string linkTitle;
    };

protected:
    const Config                     config;
    const std::shared_ptr< iLogger > logger;
    const TEventFunction             eventFunction;

    void pushEventFunction() {
        try {
            eventFunction();
        } catch (const std::exception& e) {
            logger->error("{} {}", config.linkTitle, e.what());
        }
    }

public:
    virtual void push() = 0;

    iEventChannel(const std::shared_ptr< iLogger >& logger, const Config& config, const TEventFunction& eventFunction)
        : config(config), logger(logger), eventFunction(eventFunction) {}
    virtual ~iEventChannel() = default;
};
