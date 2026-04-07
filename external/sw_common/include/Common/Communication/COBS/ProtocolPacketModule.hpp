/**
 * @file protocol_packet.h
 * @author Ruslan L.
 * @brief Header for the protocol packet
 *
 * This header defines the abstract features of the packet by protocol
 *
 * @copyright Copyright (c) 2024
**/

#pragma once

#include <stdint.h>
#include <iomanip>
#include <mutex>
#include <type_traits>
#include <vector>

#include "Common/Communication/COBS/protocol.h"
#include "Common/Communication/exception_communication.h"
#include "Common/Module/Callback/MultiCallback.hpp"

#include "Common/Module/iModule.hpp"

template < typename TPacket > class ProtocolPacketModule : iModule {
public:
    using TypePacket = TPacket;

    struct Config {
        uint8_t type;
        uint8_t addr;
    };

private:
    const int EXTRA_COBS_SIZE     = 1;
    const int EXTRA_PROTOCOL_SIZE = 6;

    protocol_packet_t protocol;
    TPacket           packet;

public:
    MultiCallback< TPacket > callbackUpdate;

public:
    ProtocolPacketModule(std::shared_ptr< iModuleLogger >& logger, Config config)
        : iModule(logger), packet{}, callbackUpdate(this->logger) {
        protocol.type     = config.type;
        protocol.addr     = config.addr;
        protocol.data_len = sizeof(TPacket);
        protocol.data     = &packet;

        if constexpr (std::is_empty_v< TPacket >)
            protocol.data_len = 0;
    }

    std::vector< uint8_t > encode() {

        std::vector< uint8_t > protocol_data(protocol_size());
        size_t                 protocol_encode_size;

        protocol_ret_code_t protocol_ret_code =
                protocol_packet_encode(&protocol, protocol_data.data(), &protocol_encode_size);

        if (protocol_ret_code != PROTOCOL_SUCCESS)
            throw comm_exc::data_is_corrupted(protocol_ret_code);

        protocol_data.back() = 0x00;

        return protocol_data;
    }

    bool decode(const protocol_packet_t& decodeProtocol) {

        if (decodeProtocol.addr == protocol.addr && decodeProtocol.type == protocol.type &&
            decodeProtocol.data_len >= protocol.data_len) {

            memcpy(protocol.data, decodeProtocol.data, protocol.data_len);

            callbackUpdate.notify(std::make_unique< TPacket >(packet));

            return true;
        }
        return false;
    }

    void setPacket(const TPacket& packet) {
        this->packet = packet;
        callbackUpdate.notify(std::make_unique< TPacket >(packet));
    }
    const TPacket getPacket() const { return packet; }

private:
    size_t structure_size() { return sizeof(TPacket); }
    size_t protocol_size() { return sizeof(TPacket) + EXTRA_COBS_SIZE + EXTRA_PROTOCOL_SIZE; }

    void reset() { std::memset(&packet, 0, sizeof(packet)); }
};
