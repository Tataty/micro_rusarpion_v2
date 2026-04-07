#pragma once

#include "Common/Logger/iModuleLogger.hpp"

class TestModuleLogger : public iModuleLogger {
private:
    void _log(const std::string& message, LogLevel logLevel) const override {}

public:
    TestModuleLogger(const std::string& name, LogLevel logLevel = LogLevel::DEBUG) : iModuleLogger(name, logLevel) {}
};

class TestLogger : public iLogger {
private:
    void _log(const std::string& message, LogLevel logLevel) const override {}

public:
    TestLogger(LogLevel logLevel = LogLevel::DEBUG) : iLogger(logLevel) {}
};