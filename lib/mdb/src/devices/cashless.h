#ifndef MDB_CASHLESS_H_INCLUDED
#define MDB_CASHLESS_H_INCLUDED


#include <stdint.h>
#include "../mdb.h"


#define MDB_CASHLESS_ADDRESS_1               0x10
#define MDB_CASHLESS_ADDRESS_2               0x60
#define MDB_CASHLESS_NON_RESPONSE_TIMEOUT_MS 5000


typedef enum {
    MDB_CASHLESS_FEATURE_LEVEL_1 = 1,
    MDB_CASHLESS_FEATURE_LEVEL_2 = 2,
    MDB_CASHLESS_FEATURE_LEVEL_3 = 3,
} mdb_cashless_feature_level_t;


typedef enum {
    MDB_CASHLESS_STATE_INACTIVE = 0,
    MDB_CASHLESS_STATE_DISABLED,
    MDB_CASHLESS_STATE_ENABLED,
    MDB_CASHLESS_SESSION_IDLE,
    MDB_CASHLESS_VEND,
    MDB_CASHLESS_REVALUE,
    MDB_CASHLESS_NEGATIVE_VEND,
} mdb_cashless_state_t;


typedef enum {
    MDB_CASHLESS_COMMAND_RESET = 0,
    MDB_CASHLESS_COMMAND_SETUP = 1,
} mdb_cashless_command_t;


typedef enum {
    MDB_CASHLESS_POLL_CODE_JUST_RESET         = 0,
    MDB_CASHLESS_POLL_CODE_READER_CONFIG_INFO = 1,
} mdb_cashless_poll_code_t;


typedef struct {
    uint8_t                      address;
    uint8_t                      columns_on_display;
    uint8_t                      rows_on_display;
    uint8_t                      ascii;
    mdb_cashless_feature_level_t feature_level;

    uint8_t configured;
    uint8_t scale_factor;
    uint8_t decimal_places;
    uint8_t maximum_response_time_seconds;

    mdb_cashless_command_t last_command;
} mdb_cashless_t;


uint8_t mdb_cashless_init(mdb_cashless_t *cashless, mdb_request_t *request, uint8_t address);
uint8_t mdb_cashless_manage_response(mdb_cashless_t *cashless, mdb_request_t *request,
                                     mdb_peripheral_response_t response);


#endif
