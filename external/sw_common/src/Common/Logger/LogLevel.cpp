#include "Common/Logger/LogLevel.hpp"

std::string toString(LogLevel logLevel) {
    switch (logLevel) {
    case LogLevel::TRACE:
        return "TRACE";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::CRITICAL:
        return "CRITICAL";
    default:
        return "NONE";
    }
}

std::string toStringAlignedByWidth(LogLevel logLevel) {
    switch (logLevel) {
    case LogLevel::TRACE:
        return "TRACE";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO ";
    case LogLevel::WARNING:
        return "WARN ";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::CRITICAL:
        return "CRIT ";
    default:
        return "NONE ";
    }
}

void fromString(LogLevel& logLevel, const std::string& str) {
    std::string lowerString = toLower(str);

    if (lowerString == "trace") {
        logLevel = LogLevel::TRACE;
    } else if (lowerString == "debug") {
        logLevel = LogLevel::DEBUG;
    } else if (lowerString == "info") {
        logLevel = LogLevel::INFO;
    } else if (lowerString == "warning") {
        logLevel = LogLevel::WARNING;
    } else if (lowerString == "error") {
        logLevel = LogLevel::ERROR;
    } else if (lowerString == "critical") {
        logLevel = LogLevel::CRITICAL;
    } else {
        throw std::invalid_argument("Unknown LogLevel string: " + lowerString);
    }
}

bool isInclude(LogLevel current, LogLevel preset) { return static_cast< int >(current) >= static_cast< int >(preset); }

std::ostream& (*toColor(LogLevel logLevel))(std::ostream&) {
    switch (logLevel) {
    case LogLevel::TRACE:
        return termcolor::grey;
    case LogLevel::DEBUG:
        return termcolor::cyan;
    case LogLevel::INFO:
        return termcolor::green;
    case LogLevel::WARNING:
        return termcolor::yellow;
    case LogLevel::ERROR:
        return termcolor::red;
    case LogLevel::CRITICAL:
        return termcolor::magenta;
    default:
        return termcolor::white;
    }
}