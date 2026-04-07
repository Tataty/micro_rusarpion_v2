/**
 * @file protocol.h
 * @author Maxinsan (maks.mashentcov1997@gmail.com)
 * @brief The header of the communication protocol module
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "COBS.h"
#include "Crc16XModem.h"

#define PROTOCOL_INPUT_BUFFER_SIZE 0xFF
#define PROTOCOL_COBS_BUFFER_SIZE  0xFF + 0x7F

typedef enum {
    PROTOCOL_PACKET_INVALID_PARAMS = -5,
    PROTOCOL_PACKET_NOT_FOUND,
    PROTOCOL_PACKET_WRONG_CRC,
    PROTOCOL_PACKET_WRONG_COBS,
    PROTOCOL_SUCCESS = 0,
    PROTOCOL_PACKET_CORRECT,
} protocol_ret_code_t;

#define PROTOCOL_TOSTRING(s) #s
#define PROTOCOL_GET_RET_CODE_NAME(code)                              \
    ({                                                                \
        const char *name = "UNDEFINED";                               \
        switch (code) {                                               \
        case PROTOCOL_PACKET_INVALID_PARAMS:                          \
            name = PROTOCOL_TOSTRING(PROTOCOL_PACKET_INVALID_PARAMS); \
            break;                                                    \
        case PROTOCOL_PACKET_NOT_FOUND:                               \
            name = PROTOCOL_TOSTRING(PROTOCOL_PACKET_NOT_FOUND);      \
            break;                                                    \
        case PROTOCOL_PACKET_WRONG_CRC:                               \
            name = PROTOCOL_TOSTRING(PROTOCOL_PACKET_WRONG_CRC);      \
            break;                                                    \
        case PROTOCOL_PACKET_WRONG_COBS:                              \
            name = PROTOCOL_TOSTRING(PROTOCOL_PACKET_WRONG_COBS);     \
            break;                                                    \
        case PROTOCOL_SUCCESS:                                        \
            name = PROTOCOL_TOSTRING(PROTOCOL_SUCCESS);               \
            break;                                                    \
        case PROTOCOL_PACKET_CORRECT:                                 \
            name = PROTOCOL_TOSTRING(PROTOCOL_PACKET_CORRECT);        \
            break;                                                    \
        default:                                                      \
            break;                                                    \
        }                                                             \
        name;                                                         \
    })

typedef struct {
    uint8_t type;
    uint8_t addr;
    uint8_t data_len;
    void   *data;
} protocol_packet_t;

/**
 * @brief The function of getting the next packet from internal buffer
 * 
 * @param[in] handle The pointer of the protocol handle
 * @param[out] packet The pointer of the packet
 * @return protocol_ret_code_t 
 */
protocol_ret_code_t protocol_get_next_packet(protocol_packet_t *packet, uint8_t *raw_data, size_t size);

/**
 * @brief The function of encode the packet and write to the external buffer
 * 
 * @param handle The pointer of the protocol handle
 * @param packet The pointer of the packet
 * @param raw_buffer The pointer of the raw buffer
 * @param size The pointer of the size of the data on the raw_buffer
 * @return protocol_ret_code_t
 */
protocol_ret_code_t protocol_packet_encode(protocol_packet_t *packet, uint8_t *raw_buffer, size_t *size);

static inline void protocol_dump_hex(const void *data, size_t size) {
    char   ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    printf("HEXDUMP:\r\n");
    for (i = 0; i < size; ++i) {
        printf("%02X ", ((unsigned char *)data)[i]);
        if (((unsigned char *)data)[i] >= ' ' && ((unsigned char *)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char *)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i + 1) % 8 == 0 || i + 1 == size) {
            printf(" ");
            if ((i + 1) % 16 == 0) {
                printf("|  %s \n", ascii);
            } else if (i + 1 == size) {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8) {
                    printf(" ");
                }
                for (j = (i + 1) % 16; j < 16; ++j) {
                    printf("   ");
                }
                printf("|  %s \n", ascii);
            }
        }
    }
}

#endif /*PROTOCOL_H*/
