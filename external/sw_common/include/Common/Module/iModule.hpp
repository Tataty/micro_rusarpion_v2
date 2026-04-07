/**
 * @file iModule.hpp
 * @author Ruslan L.
 * @brief Abstract module header with logger
 *
 * @copyright Copyright (c) 2025
 **/

#pragma once

#include <atomic>
#include <memory>

#include "Common/Logger/iModuleLogger.hpp"

class iModule {
protected:
    std::shared_ptr< iModuleLogger > logger;
    std::atomic< bool >              isInit;

    virtual void init() {}
    virtual void stop() {}

public:
    iModule(std::shared_ptr< iModuleLogger >& logger) : logger(std::move(logger)) {}

    virtual void run() {
        if (isInit)
            return;

        init();
        isInit = true;
    }
    virtual void kill() {
        if (!isInit)
            return;

        stop();
        isInit = false;
    }

    bool isWork() const { return isInit.load(); }

    virtual ~iModule() = default;
};