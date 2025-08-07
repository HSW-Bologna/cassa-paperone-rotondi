#ifndef MDB_H_INCLUDED
#define MDB_H_INCLUDED


#include <stdint.h>
#include <stdlib.h>


#define MDB_REQUEST_MAX_DATA_SIZE          34
#define MDB_REQUEST_MAX_SIZE               (MDB_REQUEST_MAX_DATA_SIZE + 2)
#define MDB_RESPONSE_TIME_MS               5
#define MDB_ADDRESS_BYTE(Address, Command) (0x100 & ((Address)&0xF1) & (Command))
#define MDB_ACK                            0x0000
#define MDB_RET                            0x00AA
#define MDB_NAK                            0x00FF


typedef enum {
    MDB_RESULT_OK = 0,
    MDB_RESULT_INVALID,
    MDB_RESULT_WRONG_CRC,
    MDB_RESULT_NOT_ENOUGH_DATA,
} mdb_result_t;

typedef enum {
    MDB_PERIPHERAL_RESPONSE_TAG_ACK = 0,
    MDB_PERIPHERAL_RESPONSE_TAG_NAK,
    MDB_PERIPHERAL_RESPONSE_TAG_DATA,
} mdb_peripheral_response_tag_t;

typedef struct {
#define MDB_PERIPHERAL_RESPONSE_MAX_DATA_SIZE 35
#define MDB_PERIPHERAL_RESPONSE_MAX_SIZE      (MDB_PERIPHERAL_RESPONSE_MAX_DATA_SIZE + 1)

    mdb_peripheral_response_tag_t tag;
    uint16_t                      data_length;
    uint8_t                       data[MDB_PERIPHERAL_RESPONSE_MAX_DATA_SIZE];
} mdb_peripheral_response_t;


typedef struct {
    uint8_t  address;
    uint8_t  command;
    uint16_t data_length;
    uint8_t  data[MDB_PERIPHERAL_RESPONSE_MAX_DATA_SIZE];
    uint16_t non_response_timeout_ms;
} mdb_request_t;


mdb_result_t mdb_parse_peripheral_response(mdb_peripheral_response_t *response, uint16_t *buffer, size_t len);
size_t       mdb_serialize_request(uint16_t *buffer, mdb_request_t request);


#endif
