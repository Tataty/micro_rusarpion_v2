#pragma once
#include <condition_variable>
#include <thread>

#include "Common/Module/Event/EventPassLogger.hpp"
#include "Common/Module/Event/iEventChannel.hpp"

class ThreadEventChannel : public iEventChannel {
protected:
    EventPassLogger passLogger;

    std::mutex              mutex;
    std::condition_variable condition;

    bool alreadySent;
    bool isWork;

    std::thread thread;

public:
    void push() override {
        passLogger.log();

        {
            std::lock_guard< std::mutex > lock(mutex);

            if (!alreadySent) {
                passLogger.pass();
                return;
            }

            alreadySent = false;
        }

        condition.notify_one();
    }

    ThreadEventChannel(const std::shared_ptr< iLogger >& logger,
                          const Config&                     config,
                          const TEventFunction&          eventFunction)
        : iEventChannel(logger, config, eventFunction),
          passLogger(logger, config.linkTitle),
          alreadySent(true),
          isWork(true),
          thread(std::bind(&ThreadEventChannel::work, this)) {}

    ~ThreadEventChannel() {
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
                return !alreadySent || !isWork;
            });

            if (!isWork)
                break;

            {
                lock.unlock();

                this->pushEventFunction();

                lock.lock();
            }

            alreadySent = true;
        }
    }
};