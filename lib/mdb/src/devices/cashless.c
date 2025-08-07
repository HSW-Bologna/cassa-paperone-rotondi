#include "../mdb.h"
#include "cashless.h"


uint8_t mdb_cashless_init(mdb_cashless_t *cashless, mdb_request_t *request, uint8_t address) {
    cashless->address      = address;
    cashless->last_command = MDB_CASHLESS_COMMAND_RESET;
    cashless->configured   = 0;

    request->command     = MDB_CASHLESS_COMMAND_RESET;
    request->data_length = 0;

    return 1;
}


uint8_t mdb_cashless_manage_response(mdb_cashless_t *cashless, mdb_request_t *request,
                                     mdb_peripheral_response_t response) {
    request->address                 = cashless->address;
    request->non_response_timeout_ms = MDB_CASHLESS_NON_RESPONSE_TIMEOUT_MS;

    if (response.data_length > 0) {
        switch (response.data[0]) {
            case MDB_CASHLESS_POLL_CODE_JUST_RESET: {
                size_t i = 0;

                cashless->last_command = MDB_CASHLESS_COMMAND_SETUP;

                request->command     = MDB_CASHLESS_COMMAND_SETUP;
                request->data[i++]   = 0;
                request->data[i++]   = cashless->feature_level;
                request->data[i++]   = cashless->columns_on_display;
                request->data[i++]   = cashless->rows_on_display;
                request->data[i++]   = cashless->ascii;
                request->data_length = i;
                return 1;
            }

            case MDB_CASHLESS_POLL_CODE_READER_CONFIG_INFO: {
                if (response.data_length < 7) {
                    // Invalid data
                    return 0;
                } else {
                    cashless->configured                    = 1;
                    cashless->feature_level                 = response.data[1];
                    cashless->scale_factor                  = response.data[4];
                    cashless->decimal_places                = response.data[5];
                    cashless->maximum_response_time_seconds = response.data[6];
                    if (cashless->maximum_response_time_seconds < MDB_CASHLESS_NON_RESPONSE_TIMEOUT_MS / 1000) {
                        cashless->maximum_response_time_seconds = MDB_CASHLESS_NON_RESPONSE_TIMEOUT_MS / 1000;
                    }
                }
                return 0;
            }

            default: {
                return 0;
            }
        }
    } else {
        switch (cashless->last_command) {
            default: {
                return 0;
            }
        }
    }
}
