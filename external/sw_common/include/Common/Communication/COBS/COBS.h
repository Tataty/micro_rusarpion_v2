#pragma once

#include <stdint.h>

uint16_t COBS_DecodeData(const uint8_t *src, uint16_t length, uint8_t *dst);
uint16_t COBS_EncodeData(const uint8_t *src, uint16_t length, uint8_t *dst);
