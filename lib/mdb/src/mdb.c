#include <stdlib.h>
#include "mdb.h"


#define MODE(Word)    ((Word) | 0x100)
#define ADDR(Word)    MODE(Word)
#define IS_MODE(Word) (((Word)&0x100) > 0)
#define IS_CHK(Word)  IS_MODE(Word)
#define IS_ADDR(Word) IS_MODE(Word)


static uint8_t crc8(uint16_t *data, size_t len);


mdb_result_t mdb_parse_peripheral_response(mdb_peripheral_response_t *response, uint16_t *buffer, size_t len) {
    if (len < 1) {
        return MDB_RESULT_NOT_ENOUGH_DATA;
    } else {
        switch (buffer[0]) {
            case MODE(MDB_ACK): {
                response->tag = MDB_PERIPHERAL_RESPONSE_TAG_ACK;
                return MDB_RESULT_OK;
            }

            case MODE(MDB_NAK): {
                response->tag = MDB_PERIPHERAL_RESPONSE_TAG_NAK;
                return MDB_RESULT_OK;
            }

            default: {
                uint16_t calculated_crc = crc8(&buffer[1], len - 1);
                uint16_t read_crc       = buffer[len - 1];

                if (calculated_crc != read_crc) {
                    return MDB_RESULT_WRONG_CRC;
                } else {
                    response->tag = MDB_PERIPHERAL_RESPONSE_TAG_DATA;

                    for (size_t i = 0; i < len - 1; i++) {
                        if (IS_MODE(buffer[i])) {
                            return MDB_RESULT_INVALID;
                        } else {
                            response->data[i] = buffer[i];
                        }
                    }
                    response->data_length = len - 1;

                    return MDB_RESULT_OK;
                }
            }
        }
    }
}


size_t mdb_serialize_request(uint16_t *buffer, mdb_request_t request) {
    buffer[0] = MDB_ADDRESS_BYTE(request.address, request.command);
    size_t i  = 0;
    for (i = 0; i < request.data_length && i < MDB_REQUEST_MAX_DATA_SIZE; i++) {
        buffer[1 + i] = request.data[i];
    }
    buffer[i + 1] = crc8(buffer, i);
    return i + 2;
}


static uint8_t crc8(uint16_t *data, size_t len) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; i++) {
        crc += (uint8_t)(data[i] & 0xFF);
    }
    return crc;
}
