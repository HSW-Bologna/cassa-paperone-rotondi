#include "driver.h"


uint8_t  i;
uint8_t  buf[25];
uint16_t crc;
char     crc_input[] = "libdriver";
uint8_t  in_buf[64];
uint8_t  in_len;
uint8_t  out_len;
uint8_t  out_buf[64];


static void a_callback(uint16_t type) {
    switch (type) {
        case MFRC522_INTERRUPT_MFIN_ACT: {
            mfrc522_interface_debug_print("mfrc522: irq mfin act.\n");

            break;
        }
        case MFRC522_INTERRUPT_CRC: {
            break;
        }
        case MFRC522_INTERRUPT_TX: {
            break;
        }
        case MFRC522_INTERRUPT_RX: {
            break;
        }
        case MFRC522_INTERRUPT_IDLE: {
            break;
        }
        case MFRC522_INTERRUPT_HI_ALERT: {
            break;
        }
        case MFRC522_INTERRUPT_LO_ALERT: {
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


int bsp_mfrc522_init(void) {
    /* No interrupt */
    // g_gpio_irq = mfrc522_interrupt_irq_handler;

    /* init */
    int res = mfrc522_basic_init(MFRC522_INTERFACE_SPI, 0x00, a_callback);
    if (res != 0) {
        return 1;
    }

    /* get the random */
    res = mfrc522_basic_generate_random(buf);
    if (res != 0) {
        return 1;
    }
    for (i = 0; i < 25; i++) {
        mfrc522_interface_debug_print("0x%02X ", buf[i]);
    }
    mfrc522_interface_debug_print("\n");


    /* calculate the crc */
    res = mfrc522_basic_calculate_crc((uint8_t *)crc_input, (uint8_t)strlen(crc_input), &crc);
    if (res != 0) {
        return 1;
    }
    mfrc522_interface_debug_print("crc is 0x%04X.\n", crc);


    /* set the input */
    in_buf[0] = 0x26;
    in_len    = 1;
    out_len   = 64;
    res       = mfrc522_basic_transceiver(in_buf, in_len, out_buf, &out_len);
    if (res != 0) {
        return 1;
    }
    for (i = 0; i < 25; i++) {
        mfrc522_interface_debug_print("0x%02X ", out_buf[i]);
    }
    mfrc522_interface_debug_print("\n");

    /* deinit */
    (void)mfrc522_basic_deinit();

    return 0;
}
