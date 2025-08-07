#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include "uart_mdb.h"
#include "mdb.h"


#define RX_BUFFER_SIZE MDB_PERIPHERAL_RESPONSE_MAX_SIZE

LOG_MODULE_REGISTER(uart_mdb);


static void                  uart_interrupt_cb(const struct device *dev, void *user_data);
static bsp_uart_mdb_result_t uart_send(uint16_t *buffer, size_t len, timestamp_t timeout);


static const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart_mdb));
K_MSGQ_DEFINE(responseq, sizeof(mdb_peripheral_response_t), 4, 1);
K_MUTEX_DEFINE(mutex);

static struct {
    timestamp_t rx_buffer_ts;
    size_t      rx_buffer_index;
    uint16_t    rx_buffer[RX_BUFFER_SIZE];

    uint8_t     pending_response;
    uint8_t     retried;
    size_t      tx_buffer_length;
    uint16_t    tx_buffer[MDB_REQUEST_MAX_SIZE];
    timestamp_t response_timeout;

    // Mutexed fields
    timestamp_t response_ts;
    timestamp_t request_ts;
} state = {0};


bsp_uart_mdb_result_t bsp_uart_mdb_manage(mdb_peripheral_response_t *response) {
    if (state.pending_response) {
        if (state.retried) {
            state.request_ts = timestamp_get();
            state.retried    = 0;
        }

        // General timeout; we should reset the device
        if (timestamp_is_expired(state.response_ts, state.response_timeout)) {
            state.pending_response = 0;
            return BSP_UART_MDB_RESULT_TIMEOUT;
        }
        // Temporary timeout
        else if (timestamp_is_expired(state.request_ts, MDB_RESPONSE_TIME_MS)) {
            // send the message again
            for (size_t i = 0; i < state.tx_buffer_length; i++) {
                uart_poll_out_u16(uart, state.tx_buffer[i]);
            }
            state.request_ts = timestamp_get();
            return BSP_UART_MDB_RESULT_BUSY;
        }
        // Waiting
        else {
            return BSP_UART_MDB_RESULT_BUSY;
        }
    }
    // Nothing going on
    else {
        return BSP_UART_MDB_RESULT_NOTHING;
    }
}


size_t bsp_uart_mdb_recv(const struct device *uart, struct ring_buf *ring_buf, uint8_t *buffer, size_t len,
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


bsp_uart_mdb_result_t bsp_uart_mdb_send(mdb_request_t *request) {
    uint16_t buffer[MDB_REQUEST_MAX_SIZE] = {0};

    buffer[0] = MDB_ADDRESS_BYTE(request->address, request->command);

    for (size_t i = 0; i < request->data_length; i++) {
        buffer[i + 1] = request->data[i];
    }
    return uart_send(buffer, 1 + request->data_length, request->non_response_timeout_ms);
}


void bsp_uart_mdb_init(void) {
    int ret = uart_irq_callback_user_data_set(uart, uart_interrupt_cb, NULL);
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
    (void)user_data;

    if (!uart_irq_update(dev)) {
        return;
    }

    if (!uart_irq_rx_ready(dev)) {
        return;
    }

    if (timestamp_is_expired(state.rx_buffer_ts, 2)) {
        state.rx_buffer_index = 0;
    }
    state.rx_buffer_ts = timestamp_get();

    /* read until FIFO empty */
    while (state.rx_buffer_index < RX_BUFFER_SIZE &&
           uart_fifo_read_u16(dev, &state.rx_buffer[state.rx_buffer_index], 1) == 1) {
        state.rx_buffer_index++;
    }

    mdb_peripheral_response_t response = {0};
    switch (mdb_parse_peripheral_response(&response, state.rx_buffer, state.rx_buffer_index)) {
        case MDB_RESULT_NOT_ENOUGH_DATA: {
            // wait
            break;
        }

        case MDB_RESULT_INVALID: {
            // Flush
            state.rx_buffer_index = 0;
            break;
        }

        case MDB_RESULT_WRONG_CRC: {
            // Flush
            state.rx_buffer_index = 0;
            uart_poll_out_u16(uart, MDB_RET);
            state.retried = 1;
            break;
        }

        case MDB_RESULT_OK: {
            while (k_msgq_put(&responseq, &response, K_NO_WAIT) != 0) {
                /* message queue is full: purge old data & try again */
                k_msgq_purge(&responseq);
            }
            state.rx_buffer_index = 0;

            uart_poll_out_u16(uart, MDB_ACK);
            break;
        }
    }
}


static bsp_uart_mdb_result_t uart_send(uint16_t *buffer, size_t len, timestamp_t timeout) {
    if (state.pending_response) {
        return BSP_UART_MDB_RESULT_BUSY;
    } else {
        for (size_t i = 0; i < len; i++) {
            state.tx_buffer[i] = buffer[i];
            uart_poll_out_u16(uart, buffer[i]);
        }
        state.tx_buffer_length = len;
        state.response_ts      = timestamp_get();
        state.request_ts       = timestamp_get();
        state.response_timeout = timeout;
        return BSP_UART_MDB_RESULT_NOTHING;
    }
}
