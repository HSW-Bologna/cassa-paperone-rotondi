#ifndef BSP_UART_MDB_H_INCLUDED
#define BSP_UART_MDB_H_INCLUDED


#include <stdint.h>
#include <stdlib.h>
#include "services/timestamp.h"
#include "mdb.h"


typedef enum {
    BSP_UART_MDB_RESULT_NOTHING = 0,
    BSP_UART_MDB_RESULT_TIMEOUT,
    BSP_UART_MDB_RESULT_RESPONSE,
    BSP_UART_MDB_RESULT_BUSY,
    BSP_UART_MDB_RESULT_INVALID_PARAMETERS,
} bsp_uart_mdb_result_t;


void                  bsp_uart_mdb_init(void);
bsp_uart_mdb_result_t bsp_uart_mdb_manage(mdb_peripheral_response_t *response);
bsp_uart_mdb_result_t bsp_uart_mdb_send(mdb_request_t *request);

#endif
