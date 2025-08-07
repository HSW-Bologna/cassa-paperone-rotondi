#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "bsp/heartbit.h"
#include "bsp/rs232_client.h"
#include "bsp/uart_mdb.h"
#include "bsp/uart_cctalk.h"
#include "bsp/digital_coin_reader.h"
#include "bsp/mfrc522/mfrc522.h"
#include "controller/controller.h"


LOG_MODULE_REGISTER(main);


int main(void) {
    bsp_heartbit_init();
    bsp_rs232_client_init();
    bsp_digital_coin_reader_init();
    bsp_mfrc522_init();
    bsp_uart_mdb_init();
    bsp_uart_cctalk_init();

    controller_init();

    for (;;) {
        controller_manage();
        k_msleep(1);
    }

    return 0;
}
