#pragma once

#include <termcolor/termcolor.hpp>
#include "Common/Text.hpp"

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

std::string toString(LogLevel logLevel);
std::string toStringAlignedByWidth(LogLevel logLevel);
void        fromString(LogLevel& logLevel, const std::string& str);

bool isInclude(LogLevel current, LogLevel preset);

std::ostream& (*toColor(LogLevel logLevel))(std::ostream&);