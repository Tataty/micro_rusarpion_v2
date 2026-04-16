#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Common/JSON/Chrono.hpp"
#include "Common/Module/Callback/MultiCallback.hpp"
#include "Common/Module/DConfig.hpp"
#include "Common/Module/iActiveModule.hpp"
#include "Common/Utils/DelayStabilizer.hpp"

template < typename TData > class DataRateStabilizerModule : public iActiveModule {
public:
    struct Config {
        std::chrono::duration< double > dataPeriod;

        auto operator<=>(const Config&) const = default;

        friend void from_json(const nlohmann::json& j, Config& c) { j.at("dataPeriod").get_to(c.dataPeriod); }
    };

private:
    std::mutex mutex;

    std::shared_ptr< const TData > bufferedLatestData;
    std::condition_variable        isDataCondition;

    std::unique_ptr< DelayStabilizer > frameDelayStabilizer;

    void onConfigChanged() { frameDelayStabilizer = std::make_unique< DelayStabilizer >(config->dataPeriod); }

public:
    MultiCallback< TData > callbackData;

public:
    DConfig< Config > config;

    DataRateStabilizerModule(std::shared_ptr< iModuleLogger >& logger, const Config& config)
        : iActiveModule(logger),
          callbackData(this->logger),
          config(config, std::bind(&DataRateStabilizerModule< TData >::onConfigChanged, this)) {}
    ~DataRateStabilizerModule() override { kill(); }

    void pushData(std::shared_ptr< const TData > data) {
        std::lock_guard< std::mutex > lock(mutex);

        bufferedLatestData = data;
        isDataCondition.notify_all();
    }

protected:
    void init() override {
        std::lock_guard< std::mutex > lock(mutex);
        bufferedLatestData.reset();
        frameDelayStabilizer->resetTimer();
    }

    void stop() override {
        std::lock_guard< std::mutex > lock(mutex);
        isDataCondition.notify_all();
    }

    std::chrono::duration< double > action() override {
        std::unique_lock< std::mutex > lock(mutex);

        isDataCondition.wait(lock, [this]() {
            return bufferedLatestData || !isWork();
        });

        if (!isWork())
            return 0ms;

        callbackData.notify(std::make_unique< TData >(*bufferedLatestData));

        return frameDelayStabilizer->getDelay();
    }
};