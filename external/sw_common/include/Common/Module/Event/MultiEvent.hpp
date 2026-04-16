#pragma once

#include "Common/Module/Event/DirectEventChannel.hpp"
#include "Common/Module/Event/ThreadEventChannel.hpp"

class MultiEvent {
public:
    using TEventFunction = std::function< void() >;

private:
    std::mutex                       mutex;
    const std::shared_ptr< iLogger > logger;

    std::vector< std::unique_ptr< iEventChannel > > listOfEventChannel;

    std::string getFormatLinkTitle(const std::string& linkTitle) { return ">" + linkTitle + ">"; }

public:
    MultiEvent(const std::shared_ptr< iLogger >& logger) : logger(logger) {}

    void subscribeByDirect(const TEventFunction& eventFunction, const std::string& linkTitle) {
        std::lock_guard< std::mutex > lock(mutex);

        DirectEventChannel::Config eventConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        listOfEventChannel.emplace_back(std::make_unique< DirectEventChannel >(logger, eventConfig, eventFunction));
    }

    void subscribeByThread(const TEventFunction& eventFunction, const std::string& linkTitle) {
        std::lock_guard< std::mutex > lock(mutex);

        ThreadEventChannel::Config eventConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        listOfEventChannel.emplace_back(std::make_unique< ThreadEventChannel >(logger, eventConfig, eventFunction));
    }

    void notify() {
        std::lock_guard< std::mutex > lock(mutex);
        for (auto& sharedEventChannel : listOfEventChannel) {
            sharedEventChannel->push();
        }
    }
};