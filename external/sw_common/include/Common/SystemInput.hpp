#pragma once

#include <sys/select.h>
#include <unistd.h>
#include <iostream>

constexpr char NO_SYMBOL = '\0';

char blockCin() {
    char c;
    std::cin >> c;
    return c;
}

char noBlockCin() {
    /* If STDIN is not terminal */
    if (!isatty(STDIN_FILENO)) {
        sleep(1); /* Simply timeout for systemctl environment */
        return NO_SYMBOL;
    }

    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    struct timeval timeout{ 0, 100000 }; /* 100ms */
    int            ret = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout);

    if (ret > 0 && FD_ISSET(STDIN_FILENO, &set)) {
        return blockCin();
    }

    return NO_SYMBOL;
}