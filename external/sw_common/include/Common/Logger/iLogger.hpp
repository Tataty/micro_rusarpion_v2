#pragma once
#include <fmt/core.h>

#include <string>
#include "Common/Logger/LogLevel.hpp"

class iLogger {
private:
    LogLevel logLevel;

protected:
    virtual void _log(const std::string& message, LogLevel logLevel) const = 0;

public:
    iLogger(LogLevel logLevel) : logLevel(logLevel) {}
    virtual ~iLogger() = default;

    template < typename... Args > void log(LogLevel level, fmt::format_string< Args... > format, Args&&... args) const {
        if (!isInclude(level, this->logLevel))
            return;

        _log(fmt::format(format, std::forward< Args >(args)...), level);
    }

    void log(const std::string& message, LogLevel logLevel) const {
        if (!isInclude(logLevel, this->logLevel))
            return;

        _log(message, logLevel);
    }

    void trace(const std::string& message) const { log(message, LogLevel::TRACE); }
    void debug(const std::string& message) const { log(message, LogLevel::DEBUG); }
    void info(const std::string& message) const { log(message, LogLevel::INFO); }
    void warning(const std::string& message) const { log(message, LogLevel::WARNING); }
    void error(const std::string& message) const { log(message, LogLevel::ERROR); }
    void critical(const std::string& message) const { log(message, LogLevel::CRITICAL); }

    template < typename... Args > void trace(fmt::format_string< Args... > f, Args&&... a) const {
        log(LogLevel::TRACE, f, std::forward< Args >(a)...);
    }
    template < typename... Args > void debug(fmt::format_string< Args... > f, Args&&... a) const {
        log(LogLevel::DEBUG, f, std::forward< Args >(a)...);
    }
    template < typename... Args > void info(fmt::format_string< Args... > f, Args&&... a) const {
        log(LogLevel::INFO, f, std::forward< Args >(a)...);
    }
    template < typename... Args > void warning(fmt::format_string< Args... > f, Args&&... a) const {
        log(LogLevel::WARNING, f, std::forward< Args >(a)...);
    }
    template < typename... Args > void error(fmt::format_string< Args... > f, Args&&... a) const {
        log(LogLevel::ERROR, f, std::forward< Args >(a)...);
    }
    template < typename... Args > void critical(fmt::format_string< Args... > f, Args&&... a) const {
        log(LogLevel::CRITICAL, f, std::forward< Args >(a)...);
    }
};