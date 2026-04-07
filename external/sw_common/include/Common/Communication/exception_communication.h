/**
 * @file exception_communication.h
 * @author Ruslan L.
 * @brief Header for the header for exceptions in case of communication
 *
 * @copyright Copyright (c) 2024
**/

#pragma once

#include <cstring>
#include <exception>
#include <string>

#include "Common/Communication/COBS/protocol.h"

static std::string protocol_get_ret_code_name(protocol_ret_code_t protocol_ret_code) {
    switch (protocol_ret_code) {
    case PROTOCOL_PACKET_INVALID_PARAMS:
        return "PROTOCOL_PACKET_INVALID_PARAMS";
    case PROTOCOL_PACKET_NOT_FOUND:
        return "PROTOCOL_PACKET_NOT_FOUND";
    case PROTOCOL_PACKET_WRONG_CRC:
        return "PROTOCOL_PACKET_WRONG_CRC";
    case PROTOCOL_PACKET_WRONG_COBS:
        return "PROTOCOL_PACKET_WRONG_COBS";
    case PROTOCOL_SUCCESS:
        return "PROTOCOL_SUCCESS";
    case PROTOCOL_PACKET_CORRECT:
        return "PROTOCOL_PACKET_CORRECT";
    default:
        break;
    }
    return "Undefined";
}

namespace comm_exc {

/**
 * @brief Data is corrupted
**/
class data_is_corrupted : public std::exception {
public:
    explicit data_is_corrupted(protocol_ret_code_t protocol_ret_code) {
        message = "Data is corrupted: " + protocol_get_ret_code_name(protocol_ret_code);
    }
    const char* what() const noexcept override { return message.c_str(); }

private:
    std::string message;
};

/**
 * @brief Data is corrupted
**/
class partial_data : public std::exception {
public:
    const char* what() const noexcept override { return "Partial data"; }
};

/**
 * @brief Timeout exception
**/
class timeout : public std::exception {
public:
    const char* what() const noexcept override { return "Timeout exception"; }
};

/**
 * @brief The communication was disconnected
**/
class disconnected : public std::exception {
public:
    const char* what() const noexcept override { return "The communication was disconnected"; }
};

/**
 * @brief There is no communication connection
**/
class not_connected : public std::exception {
public:
    const char* what() const noexcept override { return "There is no communication connection"; }
};

/**
 * @brief Target device could not be found
**/
class no_target : public std::exception {
public:
    const char* what() const noexcept override { return "Target device could not be found"; }
};

/**
 * @brief Permission denied to serial port
**/
class permission_denied : public std::exception {
public:
    const char* what() const noexcept override {
        return "Permission denied to serial port ( Using: sudo adduser $USER dialout )";
    }
};

/**
 * @brief Datal error without the possibility of processing
**/
class fatal_errno : public std::exception {
public:
    /**
     * @param message Information about what was done before the error
     * @param current_errno The errno on error
    **/
    explicit fatal_errno(const std::string& message, int current_errno) {
        what_message =
                message + " with errno: " + std::to_string(current_errno) + " ( " + strerror(current_errno) + " )";
    }

    const char* what() const noexcept override { return what_message.c_str(); }

private:
    std::string what_message;
};

}// namespace comm_exc