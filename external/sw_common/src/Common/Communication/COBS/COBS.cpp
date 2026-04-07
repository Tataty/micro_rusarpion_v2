#include <stdint.h>
#include "Common/Communication/COBS/COBS.h"

#define StartBlock()  (code_ptr = dst++, code = 1)
#define FinishBlock() (*code_ptr = code)

/**
  * @brief  COBS encode algorithm
  * @note stuffs "length" bytes of data
  * 	  at the location pointed to by "src", writing
  *       the output to the location pointed to by "dst".
  * @param  src     input data pointer
  * @param  length  count of input data bytes
  * @param  dst     output data pointer
  * @retval count of output data bytes
  */
uint16_t COBS_EncodeData(const uint8_t* src, uint16_t length, uint8_t* dst) {
    const uint8_t *start = dst, *end = src + length;
    uint8_t        code, *code_ptr; /* Where to insert the leading count */

    StartBlock();
    while (src < end) {
        if (code != 0xFF) {
            uint8_t c = *src++;
            if (c != 0) {
                *dst++ = c;
                code++;
                continue;
            }
        }
        FinishBlock();
        StartBlock();
    }
    FinishBlock();
    return (dst - start) + 1;
}

/**
  * @brief  COBS decode algorithm
  * @note UnStuffData decodes "length" bytes of data at
  * 	  the location pointed to by "src", writing the
  *       output to the location pointed to  "dst".
  * @param  src     input data pointer
  * @param  length  count of input data bytes
  * @param  dst     output data pointer
  * @retval count of output data bytes
  */
uint16_t COBS_DecodeData(const uint8_t* src, uint16_t length, uint8_t* dst) {
    const uint8_t *start = dst, *end = src + length;
    uint8_t        code = 0xFF, copy = 0;

    for (; src < end; copy--) {
        if (copy != 0) {
            *dst++ = *src++;
        } else {
            if (code != 0xFF)
                *dst++ = 0;
            copy = code = *src++;
            if (code == 0)
                break; /* Source length too long */
        }
    }
    return dst - start;
}
