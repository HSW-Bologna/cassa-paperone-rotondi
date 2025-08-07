#ifndef CCTALK_H_INCLUDED
#define CCTALK_H_INCLUDED


#include <stdint.h>


#define CCTALK_MAX_DATA_SIZE 255


typedef enum {
    CCTALK_CHECK_CHECKSUM = 0,
    CCTALK_CHECK_CRC16,
} cctalk_check_t;


typedef struct {
    uint8_t        destination;
    uint8_t        source;
    uint8_t        header;
    uint8_t        data_length;
    uint8_t        data[CCTALK_MAX_DATA_SIZE];
    cctalk_check_t crc;
} cctalk_packet_t;



#endif
