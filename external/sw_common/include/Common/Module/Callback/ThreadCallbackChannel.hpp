#pragma once
#include <condition_variable>
#include <thread>

#include "Common/Module/Callback/CallbackPassLogger.hpp"
#include "Common/Module/Callback/iCallbackChannel.hpp"

template < typename TDataPtr > class ThreadCallbackChannel : public iCallbackChannel< TDataPtr > {
public:
    using typename iCallbackChannel< TDataPtr >::TCallbackFunction;
    using typename iCallbackChannel< TDataPtr >::Config;

protected:
    CallbackPassLogger passLogger;

    std::mutex              mutex;
    std::condition_variable condition;

    TDataPtr currentData;
    bool     dataAlreadySent;
    bool     isWork;

    std::thread thread;

public:
    void push(TDataPtr data) override {
        passLogger.log();

        {
            std::lock_guard< std::mutex > lock(mutex);

            if (!dataAlreadySent) {
                passLogger.pass();
                return;
            }

            currentData     = std::move(data);
            dataAlreadySent = false;
        }

        condition.notify_one();
    }

    ThreadCallbackChannel(const std::shared_ptr< iLogger >& logger,
                          const Config&                     config,
                          const TCallbackFunction&          callbackFunction)
        : iCallbackChannel< TDataPtr >(logger, config, callbackFunction),
          passLogger(logger, config.linkTitle),
          dataAlreadySent(true),
          isWork(true),
          thread(std::bind(&ThreadCallbackChannel::work, this)) {}

    ~ThreadCallbackChannel() {
        {
            std::unique_lock< std::mutex > lock(mutex);
            isWork = false;
        }

        condition.notify_one();

        if (thread.joinable())
            thread.join();
    }

private:
    void work() {
        std::unique_lock< std::mutex > lock(mutex);

        while (isWork) {
            condition.wait(lock, [this]() {
                return !dataAlreadySent || !isWork;
            });

            if (!isWork)
                break;

            {
                lock.unlock();

                this->sendDataToCallbackFunction(currentData);

                lock.lock();
            }

            dataAlreadySent = true;
        }
    }
};