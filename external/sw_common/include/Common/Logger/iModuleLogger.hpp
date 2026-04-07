#pragma once
#include <fmt/core.h>

#include "Common/Logger/iLogger.hpp"

class iModuleLogger : public iLogger {
private:
    std::string name;

protected:
    std::string formatMessageByName(const std::string& message) const { return fmt::format("[{}] {}", name, message); }

public:
    iModuleLogger(const std::string& name, LogLevel logLevel) : iLogger(logLevel), name(name) {}

    const std::string& getName() const { return name; }
};