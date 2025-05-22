#ifndef BSP_DIGITAL_READER_H_INCLUDED
#define BSP_DIGITAL_READER_H_INCLUDED


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>


typedef struct {
    struct gpio_dt_spec     input;
    struct gpio_callback    gpio_callback;
    struct k_work_delayable work;
    uint16_t                code;
    uint32_t                count;
} bsp_digital_reader_input_t;


typedef struct {
    struct gpio_dt_spec enable;

    size_t                      inputs_num;
    bsp_digital_reader_input_t *inputs;
} bsp_digital_reader_t;


void bsp_digital_reader_init(bsp_digital_reader_t *digital_reader);


#endif
