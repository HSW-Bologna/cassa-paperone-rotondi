#ifndef BSP_UART_CCTALK_H_INCLUDED
#define BSP_UART_CCTALK_H_INCLUDED


#include <stdint.h>
#include <stdlib.h>
#include "services/timestamp.h"


void   bsp_uart_cctalk_init(void);
size_t bsp_uart_cctalk_recv(uint8_t *buffer, size_t len, timestamp_t timeout);
void   bsp_uart_cctalk_send(uint8_t *buffer, size_t len);
void   bsp_uart_cctalk_flush(void);


#endif
