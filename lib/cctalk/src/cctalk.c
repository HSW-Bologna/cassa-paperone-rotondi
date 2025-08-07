#include <stdlib.h>
#include "cctalk.h"


#define METADATA_LENGTH 5


static uint8_t  crc8(uint8_t *buffer, size_t length);
static uint16_t crc16(uint8_t *buffer, size_t length);


size_t cctalk_packet_serialize(uint8_t *buffer, size_t length, cctalk_packet_t *packet) {
    size_t required_length = packet->data_length + METADATA_LENGTH;
    if (length < required_length) {
        return 0;
    }
}


static uint8_t crc8(uint8_t *buffer, size_t length) {
    uint8_t crc = 0;

    for (size_t i = 0; i < length; i++) {
        crc += buffer[i];
    }

    return (uint8_t)(256 - (uint16_t)crc);
}


static uint16_t crc16(uint8_t *buffer, size_t length) {
    uint16_t crc = 0;

    for (size_t i = 0; i < length; i++) {
        crc ^= ((uint16_t)buffer[i]) << 8;
        for (size_t bit = 0; bit < 8; bit++) {
            if ((crc & 0x8000) > 0) {
                crc = (crc << 1) ^ 0x1021;
            }
            // 0001.0000 0010.0001 = x^12 + x^5 + 1 ( + x^16 )
            else {
                crc <<= 1;
            }
        }
    }

    return crc;
}
