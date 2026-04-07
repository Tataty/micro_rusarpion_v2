/**
 * @file i_communication.h
 * @author Ruslan L.
 * @brief Header for the abstract communication
 *
 * This header defines the abstract communication module for forwarding byte vectors
 *
 * @copyright Copyright (c) 2024
**/

#pragma once

#include <stdint.h>
#include <vector>

#include "Common/Communication/exception_communication.h"

class i_communication {
public:
    virtual std::vector< uint8_t > read_buffer()                              = 0;
    virtual void                   send_buffer(std::vector< uint8_t > buffer) = 0;

    virtual void connect()    = 0;
    virtual void disconnect() = 0;

    virtual bool is_connection() = 0;
};