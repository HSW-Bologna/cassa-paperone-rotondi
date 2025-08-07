#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "uart_cctalk.h"
#include "uart_common.h"


#define RING_BUFFER_SIZE 256


static const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart_cctalk));

static uint8_t         ring_buffer_data[RING_BUFFER_SIZE] = {0};
static struct ring_buf ring_buffer                        = RING_BUF_INIT(ring_buffer_data, RING_BUFFER_SIZE);


void bsp_uart_cctalk_init(void) {
    bsp_uart_common_register(uart, &ring_buffer);
}


size_t bsp_uart_cctalk_recv(uint8_t *buffer, size_t len, timestamp_t timeout) {
    return bsp_uart_common_recv(uart, &ring_buffer, buffer, len, timeout);
}


void bsp_uart_cctalk_send(uint8_t *buffer, size_t len) {
    bsp_uart_common_send(uart, buffer, len);
}


void bsp_uart_cctalk_flush(void) {
    ring_buf_reset(&ring_buffer);
}
