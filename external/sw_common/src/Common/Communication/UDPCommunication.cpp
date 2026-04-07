#include <cstring>
#include <iostream>

#include "Common/Communication/UDPCommunication.h"

UDPCommunication::UDPCommunication(UDPCommunication::Config config)
    : config(config), udp_socket(-1), is_connected(false) {
    std::memset(&remote_addr, 0, sizeof(remote_addr));
}

UDPCommunication::~UDPCommunication() { disconnect(); }

void UDPCommunication::connect() {
    if (is_connected) {
        return;
    }

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket == -1) {
        throw std::runtime_error("Socket creation failed");
    }

    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = TIMEOUT_USEC;

    if (setsockopt(udp_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
        close(udp_socket);
        udp_socket = -1;
        throw std::runtime_error("setsockopt (SO_RCVTIMEO) failed");
    }

    if (config.listenPort.has_value()) {
        sockaddr_in local_addr;
        local_addr.sin_family      = AF_INET;
        local_addr.sin_port        = htons(config.listenPort.value());
        local_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(udp_socket, (sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
            close(udp_socket);
            udp_socket = -1;
            throw std::runtime_error("Bind failed");
        }
    }

    if (config.serverHostAddress.has_value()) {
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port   = htons(config.serverHostAddress->port);
        inet_pton(AF_INET, config.serverHostAddress->ip.c_str(), &remote_addr.sin_addr);
    }

    is_connected = true;
}

void UDPCommunication::disconnect() {
    if (!is_connected) {
        return;
    }

    if (udp_socket != -1) {
        close(udp_socket);
        udp_socket = -1;
    }

    is_connected = false;
}

bool UDPCommunication::is_connection() { return is_connected; }

void UDPCommunication::send_buffer(std::vector< uint8_t > buffer) {
    if (!config.serverHostAddress.has_value())
        throw std::runtime_error("Server host address is not configured");

    if (buffer.empty())
        throw std::runtime_error("Buffer is empty");

    if (!is_connected)
        throw std::runtime_error("Not connected");

    ssize_t bytes_sent =
            sendto(udp_socket, buffer.data(), buffer.size(), 0, (sockaddr*)&remote_addr, sizeof(remote_addr));

    if (bytes_sent == -1) {
        throw std::runtime_error("sendto failed");
    }
}

std::vector< uint8_t > UDPCommunication::read_buffer() {
    if (!config.listenPort.has_value())
        throw std::runtime_error("Lister port is not configured");

    if (!is_connected)
        throw std::runtime_error("Not connected");

    const int              BUFFER_SIZE = 4096;
    std::vector< uint8_t > receive_buffer(BUFFER_SIZE);

    sockaddr_in sender_addr;
    socklen_t   sender_addr_size = sizeof(sender_addr);

    ssize_t bytes_received =
            recvfrom(udp_socket, receive_buffer.data(), BUFFER_SIZE, 0, (sockaddr*)&sender_addr, &sender_addr_size);

    if (bytes_received > 0) {
        receive_buffer.resize(bytes_received);
        return receive_buffer;
    }
    if (bytes_received == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            throw std::runtime_error("recvfrom failed");
        }
    }

    return {};
}