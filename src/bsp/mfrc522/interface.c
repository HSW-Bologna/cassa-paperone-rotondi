#include "interface.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(mfrc522_interface);


static const struct device *mfrc522_spi = DEVICE_DT_GET(DT_NODELABEL(mfrc522_spi));
static struct spi_config    spi_cfg     = {
           .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER,
           .frequency = 1000000U,
           .cs        = SPI_CS_CONTROL_INIT(DT_NODELABEL(mfrc522_chip_select), 5),
};


/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 reset gpio init failed
 * @note   none
 */
uint8_t mfrc522_interface_reset_gpio_init(void) {
    return 0;
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 reset gpio deinit failed
 * @note   none
 */
uint8_t mfrc522_interface_reset_gpio_deinit(void) {
    return 0;
}

/**
 * @brief     interface reset gpio write
 * @param[in] value written value
 * @return    status code
 *            - 0 success
 *            - 1 reset gpio write failed
 * @note      none
 */
uint8_t mfrc522_interface_reset_gpio_write(uint8_t value) {
    return 0;
}

// Unused
uint8_t mfrc522_interface_iic_init(void) {
    return 0;
}
uint8_t mfrc522_interface_iic_deinit(void) {
    return 0;
}
uint8_t mfrc522_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    return 0;
}
uint8_t mfrc522_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    return 0;
}

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t mfrc522_interface_spi_init(void) {
    return 0;
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t mfrc522_interface_spi_deinit(void) {
    return 0;
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mfrc522_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len) {
    struct spi_buf     rx_spi_bufs    = {.buf = buf, .len = len};
    struct spi_buf_set rx_spi_buf_set = {.buffers = &rx_spi_bufs, .count = 1};

    return spi_read(mfrc522_spi, &spi_cfg, &rx_spi_buf_set);
}

/**
 * @brief     interface spi bus write
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mfrc522_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len) {
    struct spi_buf     tx_spi_bufs    = {.buf = buf, .len = len};
    struct spi_buf_set tx_spi_buf_set = {.buffers = &tx_spi_bufs, .count = 1};

    return spi_write(mfrc522_spi, &spi_cfg, &tx_spi_buf_set);
}

// Unused
uint8_t mfrc522_interface_uart_init(void) {
    return 0;
}
uint8_t mfrc522_interface_uart_deinit(void) {
    return 0;
}
uint16_t mfrc522_interface_uart_read(uint8_t *buf, uint16_t len) {
    return 0;
}
uint8_t mfrc522_interface_uart_write(uint8_t *buf, uint16_t len) {
    return 0;
}
uint8_t mfrc522_interface_uart_flush(void) {
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void mfrc522_interface_delay_ms(uint32_t ms) {
    k_msleep(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void mfrc522_interface_debug_print(const char *const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
void mfrc522_interface_receive_callback(uint16_t type) {
    switch (type) {
        case MFRC522_INTERRUPT_MFIN_ACT: {
            mfrc522_interface_debug_print("mfrc522: irq mfin act.\n");

            break;
        }
        case MFRC522_INTERRUPT_CRC: {
            mfrc522_interface_debug_print("mfrc522: irq crc.\n");

            break;
        }
        case MFRC522_INTERRUPT_TX: {
            mfrc522_interface_debug_print("mfrc522: irq tx.\n");

            break;
        }
        case MFRC522_INTERRUPT_RX: {
            mfrc522_interface_debug_print("mfrc522: irq rx.\n");

            break;
        }
        case MFRC522_INTERRUPT_IDLE: {
            mfrc522_interface_debug_print("mfrc522: irq idle.\n");

            break;
        }
        case MFRC522_INTERRUPT_HI_ALERT: {
            mfrc522_interface_debug_print("mfrc522: irq hi alert.\n");

            break;
        }
        case MFRC522_INTERRUPT_LO_ALERT: {
            mfrc522_interface_debug_print("mfrc522: irq lo alert.\n");

            break;
        }
        case MFRC522_INTERRUPT_ERR: {
            mfrc522_interface_debug_print("mfrc522: irq err.\n");

            break;
        }
        case MFRC522_INTERRUPT_TIMER: {
            mfrc522_interface_debug_print("mfrc522: irq timer.\n");

            break;
        }
        default: {
            mfrc522_interface_debug_print("mfrc522: irq unknown code.\n");

            break;
        }
    }
}
