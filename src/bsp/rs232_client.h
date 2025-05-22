#ifndef BSP_RS232_CLIENT_H_INCLUDED
#define BSP_RS232_CLIENT_H_INCLUDED


#include <stdint.h>
#include <stdlib.h>
#include "services/timestamp.h"


void   bsp_rs232_client_init(void);
size_t bsp_rs232_client_recv(uint8_t *buffer, size_t len, timestamp_t timeout);
void   bsp_rs232_client_send(uint8_t *buffer, size_t len);
void   bsp_rs232_client_flush(void);


#endif
