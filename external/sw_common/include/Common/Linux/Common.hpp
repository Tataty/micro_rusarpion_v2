#pragma once
#include <wordexp.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

std::vector< std::string > lineCmdToArgs(const std::string& cmd);
std::vector< char* >       argsStrToChar(const std::vector< std::string >& argsStr);
void                       ignoreSigpipeOnce();

#define IGNORE_RESULT(x)        \
    do {                        \
        __typeof__(x) _z = (x); \
        (void)_z;               \
    } while (0)
