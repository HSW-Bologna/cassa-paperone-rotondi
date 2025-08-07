#include "controller.h"
#include "model/model.h"
#include "adapters/modbus_server.h"
#include "bsp/uart_mdb.h"
#include "devices/cashless.h"


static mut_model_t  _model = {};
static mut_model_t *model  = &_model;


void controller_init(void) {
    modbus_server_init();

    model_init(model);

    {
        mdb_request_t request = {0};
        if (mdb_cashless_init(&model->run.cashless, &request, MDB_CASHLESS_ADDRESS_1)) {
            bsp_uart_mdb_send(&request);
        }
    }
}


void controller_manage(void) {
    modbus_server_manage(model);

    {
        mdb_peripheral_response_t response = {0};
        switch (bsp_uart_mdb_manage(&response)) {
            case BSP_UART_MDB_RESULT_RESPONSE: {
                mdb_request_t request = {0};
                if (mdb_cashless_manage_response(&model->run.cashless, &request, response)) {
                    bsp_uart_mdb_send(&request);
                }
                break;
            }

            case BSP_UART_MDB_RESULT_TIMEOUT: {
                // TODO error handling
                break;
            }

            default:
                break;
        }
    }

    {}
}
