#ifndef BSP_UART_COMMON_H_INCLUDED
#define BSP_UART_COMMON_H_INCLUDED


#include <stdint.h>
#include <stdlib.h>
#include "services/timestamp.h"


typedef struct {
    struct ring_buf *ring_buf;
    struct device   *uart;
} bsp_uart_common_async_arg_t;


void   bsp_uart_common_register(const struct device *uart, struct ring_buf *ring_buf);
size_t bsp_uart_common_recv(const struct device *uart, struct ring_buf *ring_buf, uint8_t *buffer, size_t len,
                            timestamp_t timeout);
void   bsp_uart_common_send(const struct device *uart, uint8_t *buffer, size_t len);


#endif
