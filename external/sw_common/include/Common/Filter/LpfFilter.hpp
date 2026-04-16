#pragma once

#include "iFilter.hpp"

class LpfFilter : public iFilter< double > {
public:
    struct Config {
        double Tf1 = 0;
        double Ti1 = 0.005;
        double Tf2 = 0;
        double Ti2 = 0.005;
        double Tf3 = 0;
        double Ti3 = 0;
        double ts  = 0.001;
    };

private:
    struct DerivedConfig {
        double a1;
        double a2;
        double a3;
        double a4;
        double a5;
        double a6;
        double a7;
        double a8;
        double a9;
    };

    struct InternalState {
        double previousInput;
        double previousOutput;
        double previousCm1;

        void reset(double value) {
            previousInput  = value;
            previousCm1    = value;
            previousOutput = value;
        }
    };

    Config        config;
    DerivedConfig derivedConfig;
    InternalState internalState;

    void initFilter(double newValue) override {
        internalState.reset(newValue);
        this->value = newValue;
    }
    void updateFilterValue(double newValue) override {

        double cm1 = derivedConfig.a3 * internalState.previousCm1 + derivedConfig.a1 * newValue -
                     derivedConfig.a2 * internalState.previousInput;

        this->value = derivedConfig.a6 * internalState.previousOutput + derivedConfig.a4 * cm1 -
                      derivedConfig.a5 * internalState.previousCm1;

        internalState.previousCm1    = cm1;
        internalState.previousInput  = newValue;
        internalState.previousOutput = this->value;
    }

public:
    LpfFilter(bool ignoreFirstValue, Config config) : iFilter< double >(ignoreFirstValue), config(config) {
        double num = 0, denum = 0;

        /* 1 stage */
        denum = 2.0 * config.Ti1 + config.ts;

        num              = 2.0 * config.Tf1 + config.ts;
        derivedConfig.a1 = num / denum;

        num              = 2.0 * config.Tf1 - config.ts;
        derivedConfig.a2 = num / denum;

        num              = 2.0 * config.Ti1 - config.ts;
        derivedConfig.a3 = num / denum;

        /* 2 stage */
        denum = 2.0 * config.Ti2 + config.ts;

        num              = 2.0 * config.Tf2 + config.ts;
        derivedConfig.a4 = num / denum;

        num              = 2.0 * config.Tf2 - config.ts;
        derivedConfig.a5 = num / denum;

        num              = 2.0 * config.Ti2 - config.ts;
        derivedConfig.a6 = num / denum;

        /* 3 stage */
        denum = 2.0 * config.Ti3 + config.ts;

        num              = 2.0 * config.Tf3 + config.ts;
        derivedConfig.a7 = num / denum;

        num              = 2.0 * config.Tf3 - config.ts;
        derivedConfig.a8 = num / denum;

        num              = 2.0 * config.Ti3 - config.ts;
        derivedConfig.a9 = num / denum;
    }
};