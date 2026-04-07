#pragma once

#include <vector>
#include "Common/Communication/COBS/protocol.h"
#include "Common/Communication/exception_communication.h"

class ProtocolParser {
public:
    static constexpr size_t MAX_BUFFER_SIZE = PROTOCOL_COBS_BUFFER_SIZE;

private:
    std::vector< uint8_t > decodingBuffer;
    protocol_packet_t      protocol;

public:
    ProtocolParser() {
        protocol.data_len = static_cast< uint8_t >(PROTOCOL_COBS_BUFFER_SIZE);
        protocol.data     = new uint8_t[PROTOCOL_COBS_BUFFER_SIZE];
    }
    ~ProtocolParser() { delete[] static_cast< uint8_t* >(protocol.data); }

    const protocol_packet_t& parse(const std::vector< uint8_t >& bytes) {
        if (decodingBuffer.size() > MAX_BUFFER_SIZE) {
            decodingBuffer.clear();
        }

        decodingBuffer.insert(decodingBuffer.end(), bytes.begin(), bytes.end());

        protocol_ret_code_t protocol_ret_code =
                protocol_get_next_packet(&protocol, decodingBuffer.data(), decodingBuffer.size());

        if (protocol_ret_code == PROTOCOL_PACKET_NOT_FOUND)
            throw comm_exc::partial_data();

        if (protocol_ret_code != PROTOCOL_PACKET_CORRECT)
            throw comm_exc::data_is_corrupted(protocol_ret_code);

        decodingBuffer.clear();
        return protocol;
    }
};