#pragma once
#include <memory>

#include "Common/Logger/iModuleLogger.hpp"

class ChildModuleLogger : public iModuleLogger {
private:
    const std::shared_ptr< iModuleLogger >& parentModuleLogger;

    void _log(const std::string& message, LogLevel logLevel) const override {
        parentModuleLogger->log(this->formatMessageByName(message), logLevel);
    }

public:
    ChildModuleLogger(const std::shared_ptr< iModuleLogger >& parentModuleLogger,
                      const std::string&                      name,
                      LogLevel                                logLevel = LogLevel::TRACE)
        : iModuleLogger(name, logLevel), parentModuleLogger(parentModuleLogger) {}
};