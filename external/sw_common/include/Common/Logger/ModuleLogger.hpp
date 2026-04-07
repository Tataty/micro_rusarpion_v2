#pragma once
#include <memory>
#include <vector>

#include "Common/Logger/iModuleLogger.hpp"

class ModuleLogger : public iModuleLogger {
private:
    std::vector< std::shared_ptr< iLogger > > listOfLoggers;

    void _log(const std::string& message, LogLevel logLevel) const override {
        for (auto& logger : listOfLoggers) {
            logger->log(this->formatMessageByName(message), logLevel);
        }
    }

public:
    ModuleLogger(const std::string& name, LogLevel logLevel) : iModuleLogger(name, logLevel) {}

    void addLogger(std::shared_ptr< iLogger > logger) { listOfLoggers.push_back(std::move(logger)); }
};