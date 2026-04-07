#pragma once

#include "Common/Module/Callback/DirectCallbackChannel.hpp"
#include "Common/Module/Callback/ThreadCallbackChannel.hpp"

template < typename TData > class MultiCallback {
public:
    using TUniqueCallbackFunction = std::function< void(std::unique_ptr< TData >) >;
    using TSharedCallbackFunction = std::function< void(std::shared_ptr< const TData >) >;

private:
    std::mutex                       mutex;
    const std::shared_ptr< iLogger > logger;

    std::vector< std::unique_ptr< iCallbackChannel< std::unique_ptr< TData > > > >       listOfUniqueCallbackChannel;
    std::vector< std::unique_ptr< iCallbackChannel< std::shared_ptr< const TData > > > > listOfSharedCallbackChannel;

    std::string getFormatLinkTitle(const std::string& linkTitle) { return ">" + linkTitle + ">"; }

public:
    MultiCallback(const std::shared_ptr< iLogger >& logger) : logger(logger) {}

    void subscribeToUniqueByDirect(const TUniqueCallbackFunction& callbackFunction, const std::string& linkTitle) {
        std::lock_guard< std::mutex > lock(mutex);

        typename DirectCallbackChannel< std::unique_ptr< TData > >::Config callbackDataConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        listOfUniqueCallbackChannel.emplace_back(std::make_unique< DirectCallbackChannel< std::unique_ptr< TData > > >(
                logger, callbackDataConfig, callbackFunction));
    }

    void subscribeToSharedByDirect(const TSharedCallbackFunction& callbackFunction, const std::string& linkTitle) {
        std::lock_guard< std::mutex > lock(mutex);

        typename DirectCallbackChannel< std::shared_ptr< const TData > >::Config callbackDataConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        listOfSharedCallbackChannel.emplace_back(
                std::make_unique< DirectCallbackChannel< std::shared_ptr< const TData > > >(
                        logger, callbackDataConfig, callbackFunction));
    }

    void subscribeToUniqueByThread(const TUniqueCallbackFunction& callbackFunction, const std::string& linkTitle) {
        std::lock_guard< std::mutex > lock(mutex);

        typename ThreadCallbackChannel< std::unique_ptr< TData > >::Config callbackDataConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        listOfUniqueCallbackChannel.emplace_back(std::make_unique< ThreadCallbackChannel< std::unique_ptr< TData > > >(
                logger, callbackDataConfig, callbackFunction));
    }

    void subscribeToSharedByThread(const TSharedCallbackFunction& callbackFunction, const std::string& linkTitle) {
        std::lock_guard< std::mutex > lock(mutex);

        typename ThreadCallbackChannel< std::shared_ptr< const TData > >::Config callbackDataConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        listOfSharedCallbackChannel.emplace_back(
                std::make_unique< ThreadCallbackChannel< std::shared_ptr< const TData > > >(
                        logger, callbackDataConfig, callbackFunction));
    }

    void notify(std::shared_ptr< const TData > data) {
        std::lock_guard< std::mutex > lock(mutex);

        const bool isUniqueChannels = !listOfUniqueCallbackChannel.empty();

        if (isUniqueChannels)
            throw std::runtime_error("There are unique subscribers, sharing data is not possible");

        notifyAllShared(data);
    }

    void notify(std::unique_ptr< TData > data) {
        std::lock_guard< std::mutex > lock(mutex);

        const bool isUniqueChannels = !listOfUniqueCallbackChannel.empty();
        const bool isSharedChannels = !listOfSharedCallbackChannel.empty();

        if (isUniqueChannels && isSharedChannels) {

            /* Shared channels */
            std::shared_ptr< const TData > copySharedData = std::make_shared< TData >(*data);
            notifyAllShared(copySharedData);

            /* Unique channels */
            notifyAllUnique(data);

        } else if (!isUniqueChannels && isSharedChannels) { /* Share only - no copy */

            std::shared_ptr< const TData > moveSharedData = std::move(data);
            notifyAllShared(moveSharedData);

        } else if (isUniqueChannels && !isSharedChannels) { /* Unique only */
            notifyAllUnique(data);
        }
    }

private:
    void notifyAllShared(std::shared_ptr< const TData >& data) {

        for (auto& sharedCallbackChannel : listOfSharedCallbackChannel) {
            sharedCallbackChannel->push(data);
        }
    }

    void notifyAllUnique(std::unique_ptr< TData >& data) {

        for (size_t i = 1; i < listOfUniqueCallbackChannel.size(); i++) {
            listOfUniqueCallbackChannel[i]->push(std::make_unique< TData >(*data));
        }

        listOfUniqueCallbackChannel[0]->push(std::move(data));
    }
};