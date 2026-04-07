/**
 * @file iActiveModule.hpp
 * @author Ruslan L.
 * @brief Abstract module header with init and main action
 *
 * @copyright Copyright (c) 2025
 **/

#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>
using namespace std::chrono;

#include "Common/Logger/ApsLogger.hpp"
#include "Common/Module/iModule.hpp"

class iActiveModule : protected iModule {
private:
    static constexpr std::chrono::milliseconds REINIT_DELAY = 1000ms;

    std::mutex mutex;

    std::thread         workThread;

    ApsLogger apsLogger;

public:
    iActiveModule(std::shared_ptr< iModuleLogger >& logger) : iModule(logger), apsLogger(this->logger) {
        isInit.store(false);
    }

    virtual ~iActiveModule() { kill(); }

    void run() override {
        std::lock_guard< std::mutex > lock(mutex);

        if (isInit.load() == true)
            return;

        if (workThread.joinable())
            workThread.join();

        isInit.store(true);
        workThread = std::thread(std::bind(&iActiveModule::work, this));
    }

    void kill() override {
        std::lock_guard< std::mutex > lock(mutex);

        if (isInit.load() == false)
            return;

        isInit.store(false);
        stop();

        if (workThread.joinable())
            workThread.join();

        logger->info("Work thread successfully stopped");
    }

protected:
    virtual std::chrono::duration< double > action() = 0;

private:
    void work() {
        while (isInit.load()) {

            logger->debug("Start of the work thread");

            if (!tryInitModule())
                continue;

            doWorkModule();
        }
    }

    bool tryInitModule() {
        try {

            init();

        } catch (const std::exception& e) {

            logger->error("Init failed - {}", e.what());

            std::this_thread::sleep_for(REINIT_DELAY);
            return false;
        }

        logger->info("Initialization successful");

        return true;
    }
    void doWorkModule() {
        try {
            while (isInit.load()) {

                apsLogger.logApsOnceWithDelay();
                std::chrono::duration< double > delay = action();

                if (isInit.load() == false)
                    break;

                std::this_thread::sleep_for(delay);
            }
        } catch (const std::exception& e) {
            logger->error("Unhandled exception - {}", e.what());
            stop();
            std::this_thread::sleep_for(REINIT_DELAY);
        }
    }
};