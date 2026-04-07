/**
 * @file protocol.c
 * @author Maxinsan (maks.mashentcov1997@gmail.com)
 * @brief The source of the communication protocol module
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdbool.h>
#include "Common/Communication/COBS/protocol.h"

#define PROTOCOL_START_BYTE 0x7E
#define PROTOCOL_END_BYTE   0x00

#define PROTOCOL_START_POS    0
#define PROTOCOL_TYPE_POS     1
#define PROTOCOL_ADDR_POS     2
#define PROTOCOL_DATA_LEN_POS 3
#define PROTOCOL_DATA_RAW_POS 4
#define PROTOCOL_COMMON_SIZE  6
#define PROTOCOL_OFFSET_CRC   3

#include <iostream>
#include <vector>

protocol_ret_code_t protocol_get_next_packet(protocol_packet_t* packet, uint8_t* raw_data, size_t size) {
    uint8_t temp_cobs_buf[PROTOCOL_COBS_BUFFER_SIZE] = { 0 };

    /*Check the passed parameters*/
    if (!packet || !raw_data || !size) {
        return PROTOCOL_PACKET_INVALID_PARAMS;
    }

    uint8_t* ptr_raw_data    = raw_data;
    uint8_t* ptr_header_byte = NULL;
    uint8_t* ptr_footer_byte = NULL;

    /*Try find the header byte*/
    for (size_t i = 0; i < size; i++) {
        if (raw_data[i] == PROTOCOL_START_BYTE) {
            if (i) {
                ptr_header_byte = &raw_data[i - 1];
                break;
            }
        }
    }

    if (!ptr_header_byte) {
        return PROTOCOL_PACKET_NOT_FOUND;
    }

    ptr_raw_data = ptr_header_byte;

    /*Try find the footer byte*/
    for (size_t i = 0; i < size - (ptr_header_byte - raw_data); i++) {

        if (ptr_raw_data[i] == PROTOCOL_END_BYTE) {
            ptr_footer_byte = &ptr_raw_data[i];
            break;
        }
    }

    if (!ptr_footer_byte) {
        return PROTOCOL_PACKET_NOT_FOUND;
    }

    /*Decode COBS*/
    if (!COBS_DecodeData(ptr_header_byte, (uint16_t)(int)(ptr_footer_byte - ptr_header_byte), temp_cobs_buf)) {
        return PROTOCOL_PACKET_WRONG_COBS;
    }

    /*Try parse*/
    packet->type     = temp_cobs_buf[PROTOCOL_TYPE_POS];
    packet->addr     = temp_cobs_buf[PROTOCOL_ADDR_POS];
    packet->data_len = temp_cobs_buf[PROTOCOL_DATA_LEN_POS];

    uint16_t crc_in = 0;
    memcpy(&crc_in, &temp_cobs_buf[packet->data_len + PROTOCOL_DATA_RAW_POS], sizeof(crc_in));

    /*Check the CRC*/
    uint16_t crc_ref = crc_calculate(&temp_cobs_buf[PROTOCOL_TYPE_POS],
                                     temp_cobs_buf[PROTOCOL_DATA_LEN_POS] + PROTOCOL_OFFSET_CRC);

    if (crc_ref != crc_in) {
        return PROTOCOL_PACKET_WRONG_CRC;
    }

    /*Copy payload*/
    if (packet->data_len) {
        memcpy(packet->data, &temp_cobs_buf[PROTOCOL_DATA_RAW_POS], packet->data_len);
    }

    return PROTOCOL_PACKET_CORRECT;
}

protocol_ret_code_t protocol_packet_encode(protocol_packet_t* packet, uint8_t* raw_buffer, size_t* size) {

    if (!packet || !raw_buffer || !size) {
        return PROTOCOL_PACKET_INVALID_PARAMS;
    }

    uint8_t temp_raw_buffer[PROTOCOL_COBS_BUFFER_SIZE] = {
        0,
    };

    temp_raw_buffer[PROTOCOL_START_POS]    = PROTOCOL_START_BYTE;
    temp_raw_buffer[PROTOCOL_TYPE_POS]     = packet->type;
    temp_raw_buffer[PROTOCOL_ADDR_POS]     = packet->addr;
    temp_raw_buffer[PROTOCOL_DATA_LEN_POS] = packet->data_len;

    if (packet->data && packet->data_len) {
        memcpy(&temp_raw_buffer[PROTOCOL_DATA_RAW_POS], packet->data, packet->data_len);
    }

    uint16_t crc = crc_calculate(&temp_raw_buffer[PROTOCOL_TYPE_POS], packet->data_len + PROTOCOL_OFFSET_CRC);

    memcpy(&temp_raw_buffer[PROTOCOL_DATA_RAW_POS + packet->data_len], &crc, sizeof(crc));

    *size = COBS_EncodeData(temp_raw_buffer, (packet->data_len + PROTOCOL_COMMON_SIZE), raw_buffer);

    return PROTOCOL_SUCCESS;
}