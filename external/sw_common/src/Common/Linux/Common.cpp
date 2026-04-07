#include "Common/Linux/Common.hpp"

#include <signal.h>

std::vector< std::string > lineCmdToArgs(const std::string& cmd) {
    wordexp_t p;
    if (wordexp(cmd.c_str(), &p, 0) != 0)
        throw std::runtime_error("Failed to word expansion");

    std::vector< std::string > result;
    result.reserve(p.we_wordc);

    for (size_t i = 0; i < p.we_wordc; i++) {
        result.emplace_back(p.we_wordv[i]);
    }

    wordfree(&p);
    return result;
}

std::vector< char* > argsStrToChar(const std::vector< std::string >& argsStr) {
    std::vector< char* > argsChar(argsStr.size() + 1);

    for (size_t i = 0; i < argsStr.size(); i++) {
        argsChar[i] = const_cast< char* >(argsStr[i].c_str());
    }
    argsChar[argsStr.size()] = nullptr;

    return argsChar;
}

void ignoreSigpipeOnce() {
    static bool done = false;
    if (!done) {
        signal(SIGPIPE, SIG_IGN);
        done = true;
    }
}