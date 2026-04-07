#pragma once

#include "Common/Module/Callback/DirectCallbackChannel.hpp"

template < typename TData > class SingleCallback {
public:
    using TUniqueCallbackFunction = std::function< void(std::unique_ptr< TData >) >;

private:
    const std::shared_ptr< iLogger > logger;

    std::unique_ptr< DirectCallbackChannel< std::unique_ptr< TData > > > uniqueCallbackChannel;

    std::string getFormatLinkTitle(const std::string& linkTitle) { return ">" + linkTitle + ">"; }

public:
    SingleCallback(const std::shared_ptr< iLogger >& logger) : logger(logger) {}

    void subscribeByDirect(const TUniqueCallbackFunction& callbackFunction, const std::string& linkTitle) {

        if (uniqueCallbackChannel)
            throw std::runtime_error("Subscriber to Callback already exists");

        typename DirectCallbackChannel< std::unique_ptr< TData > >::Config callbackDataConfig = {
            .linkTitle = getFormatLinkTitle(linkTitle),
        };

        uniqueCallbackChannel = std::make_unique< DirectCallbackChannel< std::unique_ptr< TData > > >(
                logger, callbackDataConfig, callbackFunction);
    }
    void subscribeToUniqueByDirect(const TUniqueCallbackFunction& callbackFunction, const std::string& linkTitle) {
        subscribeByDirect(SingleCallback< TData >::TUniqueCallbackFunction(callbackFunction), linkTitle);
    }

    void notify(std::unique_ptr< TData > data) {

        if (!uniqueCallbackChannel)
            throw std::runtime_error("There is no subscriber on Callback");

        uniqueCallbackChannel->push(std::move(data));
    }
};