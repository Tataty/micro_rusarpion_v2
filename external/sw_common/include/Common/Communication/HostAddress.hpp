#pragma once

#include <string>

struct HostAddress {
    std::string ip;   /* TODO: make uint8_t[4] */
    int         port; /* TODO: make uint16_t */
};

std::string toString(const HostAddress& ha);