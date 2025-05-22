#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include "uart_common.h"


LOG_MODULE_REGISTER(uart_common);


static void uart_interrupt_cb(const struct device *dev, void *user_data);


size_t bsp_uart_common_recv(const struct device *uart, struct ring_buf *ring_buf, uint8_t *buffer, size_t len,
                            timestamp_t timeout) {
    timestamp_t ts         = timestamp_get();
    size_t      read_bytes = 0;

    while (read_bytes < len && !timestamp_is_expired(ts, timeout)) {
        read_bytes += ring_buf_get(ring_buf, &buffer[read_bytes], len);

        if (read_bytes < len) {
            k_msleep(1);
        }
    }

    return read_bytes;
}


void bsp_uart_common_send(const struct device *uart, uint8_t *buffer, size_t len) {
    for (size_t i = 0; i < len; i++) {
        uart_poll_out(uart, buffer[i]);
    }
}


void bsp_uart_common_register(const struct device *uart, struct ring_buf *ring_buf) {
    int ret = uart_irq_callback_user_data_set(uart, uart_interrupt_cb, ring_buf);
    switch (ret) {
        case 0:
            break;

        case -ENOTSUP:
            LOG_WRN("Interrupt-driven UART API support not enabled");
            return;

        case -ENOSYS:
            LOG_WRN("UART device does not support interrupt-driven API");
            return;

        default:
            LOG_WRN("Error setting UART callback: %d\n", ret);
            return;
    }

    uart_irq_rx_enable(uart);
}



static void uart_interrupt_cb(const struct device *dev, void *user_data) {
    struct ring_buf *ring_buf = user_data;

    if (!uart_irq_update(dev)) {
        return;
    }

    if (!uart_irq_rx_ready(dev)) {
        return;
    }

    uint8_t c = 0;
    /* read until FIFO empty */
    while (uart_fifo_read(dev, &c, 1) == 1) {
        ring_buf_put(ring_buf, &c, 1);
    }
}
