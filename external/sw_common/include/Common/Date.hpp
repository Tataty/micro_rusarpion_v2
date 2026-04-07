#pragma once

#include <chrono>
#include <ctime>
#include <string>

static std::string getDateStr() {
    auto        now   = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time = std::localtime(&now_c);

    char buffer[80];

    std::strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M:%S", local_time);

    return buffer;
}