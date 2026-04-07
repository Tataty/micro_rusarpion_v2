#pragma once
#include <atomic>
#include <functional>
#include <memory>

template < typename TConfig > class DConfig {
public:
    using TOnConfigUpdateFunction = std::function< void() >;
    using TConfigPtr              = std::shared_ptr< const TConfig >;

private:
    mutable TConfigPtr      currentConfig;
    TOnConfigUpdateFunction onConfigUpdateFunction;

    void configUpdateEvent() {
        if (onConfigUpdateFunction) {
            onConfigUpdateFunction();
        }
    }

public:
    DConfig(const TConfig& initialCfg, const TOnConfigUpdateFunction& onUpdate = TOnConfigUpdateFunction{})
        : currentConfig(std::make_shared< const TConfig >(initialCfg)), onConfigUpdateFunction(std::move(onUpdate)) {
        configUpdateEvent();
    }

    void update(const TConfig& newCfg) {
        TConfigPtr oldPtr = std::atomic_load_explicit(&currentConfig, std::memory_order_acquire);

        if (oldPtr && (*oldPtr == newCfg)) {
            return;
        }

        TConfigPtr newPtr = std::make_shared< const TConfig >(newCfg);

        std::atomic_store_explicit(&currentConfig, newPtr, std::memory_order_release);

        configUpdateEvent();
    }

    template < typename TFunction > void transform(TFunction&& userTransformFunction) {
        TConfigPtr oldPtr = std::atomic_load_explicit(&currentConfig, std::memory_order_acquire);
        if (!oldPtr) {
            return;
        }

        TConfig newCfg = *oldPtr;

        userTransformFunction(newCfg);

        if (*oldPtr == newCfg) {
            return;
        }

        TConfigPtr newPtr = std::make_shared< const TConfig >(newCfg);
        std::atomic_store_explicit(&currentConfig, newPtr, std::memory_order_release);

        configUpdateEvent();
    }

    const TConfigPtr get() const { return std::atomic_load_explicit(&currentConfig, std::memory_order_acquire); }
    const TConfigPtr operator->() const { return std::atomic_load_explicit(&currentConfig, std::memory_order_acquire); }
};
