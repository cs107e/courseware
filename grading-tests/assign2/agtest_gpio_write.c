// write single pins

#include "grade_gpio.h"

void run_test(void) {
    gpio_init();

    // these pins are all intentionally chosen from the ones used in clock, all in lower bank
    const gpio_id_t pins[] = {GPIO_PB4, GPIO_PB3, GPIO_PB2, GPIO_PC0};


    for (int i = 0; i < COUNT(pins); i++) {
        gpio_id_t pin = pins[i];
        ref_gpio_set_output(pin);
        trace_write(pin, 0);
        trace_write(pin, 1);
        trace_write(pin, 0);
        trace_write(pin, 1);
    }

}
