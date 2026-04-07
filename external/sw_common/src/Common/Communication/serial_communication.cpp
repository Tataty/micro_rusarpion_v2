/**
 * @file serial_communication.cpp
 * @author Ruslan L.
 * @brief The source of the serial communication module
 * 
 * @copyright Copyright (c) 2024
**/

#include "Common/Communication/serial_communication.h"

serial_communication::serial_communication(const std::string& serial_port_str, int baud_rate)
    : port_str(serial_port_str), baud_rate(baud_rate) {}

std::vector< uint8_t > serial_communication::read_buffer() {
    if (!is_connection())
        throw comm_exc::not_connected();

    std::vector< uint8_t > read_buf(256);

    int num_bytes = read(serial_port, read_buf.data(), read_buf.size());

    if (errno == EIO || errno == ENXIO || errno == EPIPE || errno == EBADF)
        throw comm_exc::disconnected();

    if (num_bytes == 0)
        throw comm_exc::timeout();

    if (num_bytes < 0)
        throw comm_exc::fatal_errno("Read from serial port", errno);

    read_buf.resize(num_bytes);

    return read_buf;
}

void serial_communication::send_buffer(std::vector< uint8_t > buffer) {
    if (!is_connection())
        throw comm_exc::not_connected();

    int ret_code = write(serial_port, buffer.data(), buffer.size());

    if (errno == EIO || errno == ENXIO || errno == EPIPE || errno == EBADF)
        throw comm_exc::disconnected();

    if (ret_code < 0)
        throw comm_exc::fatal_errno("Write to serial port", errno);
}

void serial_communication::connect() {
    disconnect();

    serial_port = open(port_str.c_str(), O_RDWR);

    if (serial_port < 0) {
        if (errno == ENOENT)
            throw comm_exc::no_target();
        if (errno == EACCES)
            throw comm_exc::permission_denied();
        throw comm_exc::fatal_errno("The serial port cannot be open", errno);
    }

    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0) {
        disconnect();
        throw comm_exc::fatal_errno("Read in existing settings tcgetattr", errno);
    }

    tty.c_cflag &= ~PARENB;       // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;       // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;        // Clear all bits that set the data size
    tty.c_cflag |= CS8;           // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;      // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL;// Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                  // Disable echo
    tty.c_lflag &= ~ECHOE;                 // Disable erasure
    tty.c_lflag &= ~ECHONL;                // Disable new-line echo
    tty.c_lflag &= ~ISIG;                  // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);// Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                     ICRNL);// Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST;// Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR;// Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = DEFAULT_SERIAL_TIMEOUT;
    tty.c_cc[VMIN]  = 0;

    cfsetispeed(&tty, baud_rate);
    cfsetospeed(&tty, baud_rate);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        disconnect();
        throw comm_exc::fatal_errno("Save tty settings tcsetattr", errno);
    }
}

void serial_communication::disconnect() {
    if (is_connection())
        close(serial_port);
}

bool serial_communication::is_connection() { return serial_port >= 0; }