#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <inttypes.h>
#include "digital_coin_reader.h"
#include "digital_reader.h"


LOG_MODULE_REGISTER(digital_coin_reader);


static bsp_digital_reader_input_t digital_reader_inputs[BSP_DIGITAL_COIN_READER_LINES] = {
    {
        .input = GPIO_DT_SPEC_GET(DT_NODELABEL(coin_reader_line_1), gpios),
        .code  = BSP_DIGITAL_COIN_READER_LINE_1,
    },
};
static bsp_digital_reader_t digital_reader = {
    .inputs_num = BSP_DIGITAL_COIN_READER_LINES,
    .inputs     = digital_reader_inputs,
    .enable     = GPIO_DT_SPEC_GET(DT_NODELABEL(digital_coin_reader_enable), gpios),
};


void bsp_digital_coin_reader_init(void) {
    bsp_digital_reader_init(&digital_reader);
}
