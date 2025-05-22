#include "digital_reader.h"
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>


static void line_debounce_cb(struct k_work *work);
static void line_trigger_cb(const struct device *dev, struct gpio_callback *cb, uint32_t pins);


LOG_MODULE_REGISTER(digital_reader);


void bsp_digital_reader_init(bsp_digital_reader_t *digital_reader) {
    int res = 0;

    if (!gpio_is_ready_dt(&digital_reader->enable)) {
        LOG_WRN("Enable line not ready!");
        return;
    }

    if ((res = gpio_pin_configure_dt(&digital_reader->enable, GPIO_OUTPUT_ACTIVE)) > 0) {
        LOG_WRN("Inhibit line not configured: %i", res);
        return;
    }

    gpio_pin_set_dt(&digital_reader->enable, 0);

    for (size_t i = 0; i < digital_reader->inputs_num; i++) {
        bsp_digital_reader_input_t *input = &digital_reader->inputs[i];

        k_work_init_delayable(&input->work, line_debounce_cb);

        if (!gpio_is_ready_dt(&input->input)) {
            printk("Error: button device %s is not ready\n", input->input.port->name);
            return;
        }

        int ret = gpio_pin_configure_dt(&input->input, GPIO_INPUT);
        if (ret != 0) {
            printk("Error %d: failed to configure %s pin %d\n", ret, input->input.port->name, input->input.pin);
            return;
        }

        ret = gpio_pin_interrupt_configure_dt(&input->input, GPIO_INT_EDGE_TO_ACTIVE);
        if (ret != 0) {
            printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, input->input.port->name,
                   input->input.pin);
            return;
        }

        gpio_init_callback(&input->gpio_callback, line_trigger_cb, BIT(input->input.pin));
        gpio_add_callback(input->input.port, &input->gpio_callback);
    }

    LOG_INF("Initialized");
}


static void line_trigger_cb(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    (void)pins;

    bsp_digital_reader_input_t *input = CONTAINER_OF(cb, bsp_digital_reader_input_t, gpio_callback);
    k_work_reschedule(&input->work, K_MSEC(10));
}


static void line_debounce_cb(struct k_work *k_work) {
    struct k_work_delayable    *work_delayable = k_work_delayable_from_work(k_work);
    bsp_digital_reader_input_t *input          = CONTAINER_OF(work_delayable, bsp_digital_reader_input_t, work);
    input->count++;
}
