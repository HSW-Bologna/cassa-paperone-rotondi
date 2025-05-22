#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "rs232_client.h"
#include "uart_common.h"


#define RING_BUFFER_SIZE 256


static const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart_rs232_client));

static uint8_t         ring_buffer_data[RING_BUFFER_SIZE] = {0};
static struct ring_buf ring_buffer                        = RING_BUF_INIT(ring_buffer_data, RING_BUFFER_SIZE);


void bsp_rs232_client_init(void) {
    bsp_uart_common_register(uart, &ring_buffer);
}


size_t bsp_rs232_client_recv(uint8_t *buffer, size_t len, timestamp_t timeout) {
    return bsp_uart_common_recv(uart, &ring_buffer, buffer, len, timeout);
}


void bsp_rs232_client_send(uint8_t *buffer, size_t len) {
    bsp_uart_common_send(uart, buffer, len);
}


void bsp_rs232_client_flush(void) {
    ring_buf_reset(&ring_buffer);
}
