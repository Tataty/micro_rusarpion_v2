#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Common/Communication/HostAddress.hpp"
#include "Common/Communication/i_communication.h"

// TODO: refactoring
class UDPCommunication : public i_communication {
public:
    struct Config {
        std::optional< HostAddress > serverHostAddress;
        std::optional< int >         listenPort;
    };

public:
    UDPCommunication(Config config);
    ~UDPCommunication();

    std::vector< uint8_t > read_buffer() override;
    void                   send_buffer(std::vector< uint8_t > buffer) override;
    void                   connect() override;
    void                   disconnect() override;
    bool                   is_connection() override;

private:
    Config config;

    int         udp_socket;
    sockaddr_in remote_addr;
    bool        is_connected;

    static constexpr __suseconds_t TIMEOUT_USEC = 10000;
};