/**
 * @file serial_communication.h
 * @author Ruslan L.
 * @brief Module header for serial port communication
 *
 * @copyright Copyright (c) 2024
**/

#pragma once

#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "Common/Communication/i_communication.h"

class serial_communication : public i_communication {
public:
    static constexpr cc_t DEFAULT_SERIAL_TIMEOUT  = 10; /* in deciseconds */
    static constexpr int  DEFAULT_SERIAL_BAUDRATE = B115200;

    /**
     * @param serial_port_str Path to the serial port file
     * @param baud_rate Data transfer rate (by type B115200)
    **/
    serial_communication(const std::string& serial_port_str, int baud_rate = DEFAULT_SERIAL_BAUDRATE);

    ~serial_communication() { disconnect(); }

    /**
     * @brief Reads the buffer and returns it as a byte vector, waits for a timeout in case there is no data
     * 
     * @return Buffer as a byte vector
     * 
     * @throw comm_exc::not_connected
     * @throw comm_exc::disconnected
     * @throw comm_exc::timeout
     * @throw comm_exc::fatal_errno
    **/
    std::vector< uint8_t > read_buffer();

    /**
     * @brief Transmits the byte vector over the serial port
     * 
     * @param buffer Transfer buffer
     * 
     * @throw comm_exc::not_connected
     * @throw comm_exc::disconnected
     * @throw comm_exc::fatal_errno
    **/
    void send_buffer(std::vector< uint8_t > buffer);

    /**
     * @brief Connects to a previously specified serial port
     * 
     * @throw comm_exc::no_target
     * @throw comm_exc::permission_denied
     * @throw comm_exc::fatal_errno
    **/
    void connect();

    /**
     * @brief Disconnect from target
    **/
    void disconnect();

    /**
     * @brief Checks if there is a connection to the target
     * 
     * @return Boolean value of whether there is a connection
    **/
    bool is_connection();

private:
    int         serial_port = -1;
    std::string port_str;
    int         baud_rate;
};