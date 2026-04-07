#pragma once
#include <iostream>

#include "Common/Logger/iLogger.hpp"

class ConsoleLogger : public iLogger {
protected:
    void _log(const std::string& message, LogLevel logLevel) const override {
        std::cout << toColor(logLevel) << "(" << toStringAlignedByWidth(logLevel) << ") " << message << termcolor::reset
                  << std::endl;
    }

public:
    ConsoleLogger(LogLevel logLevel = LogLevel::DEBUG) : iLogger(logLevel) {}
};