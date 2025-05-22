#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(heartbit);

static void heartbit_timer(struct k_timer *timer_id);


static K_TIMER_DEFINE(timer, heartbit_timer, NULL);
static const struct gpio_dt_spec heartbit_led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_heartbit), gpios);


void bsp_heartbit_init(void) {
    int res = 0;

    if (!gpio_is_ready_dt(&heartbit_led)) {
        LOG_WRN("Heartbit led not ready!");
        return;
    }

    if ((res = gpio_pin_configure_dt(&heartbit_led, GPIO_OUTPUT_ACTIVE)) > 0) {
        LOG_WRN("Heartbit led not configured: %i", res);
        return;
    }

    k_timer_start(&timer, K_MSEC(975), K_NO_WAIT);
    LOG_INF("Initialized");
}


static void heartbit_timer(struct k_timer *timer_id) {
    uint8_t blink = k_timer_user_data_get(timer_id) != NULL;

    gpio_pin_toggle_dt(&heartbit_led);

    k_timer_user_data_set(timer_id, blink ? NULL : (void *)(uintptr_t)1);
    k_timer_start(timer_id, K_MSEC(blink ? 25 : 1975), K_NO_WAIT);
}
