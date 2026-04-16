#pragma once

#include "Common/Logger/iModuleLogger.hpp"

class TestModuleLogger : public iModuleLogger {
private:
    void _log(const std::string& message, LogLevel logLevel) const override {}

public:
    TestModuleLogger() : iModuleLogger("TEST", LogLevel::DEBUG) {}
};

class TestLogger : public iLogger {
private:
    void _log(const std::string& message, LogLevel logLevel) const override {}

public:
    TestLogger() : iLogger(LogLevel::DEBUG) {}
};