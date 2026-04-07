#include "Common/Communication/HostAddress.hpp"

std::string toString(const HostAddress& ha) { return ha.ip + ":" + std::to_string(ha.port); }