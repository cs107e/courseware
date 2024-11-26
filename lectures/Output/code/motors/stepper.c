/* File: stepper.c
 * ---------------
 * Using UNL2003 driver board
 * connected to stepper motor 28BYJ-48
 *
 * https://www.electronicoscaldas.com/datasheet/ULN2003A-PCB.pdf
 * https://www.mouser.com/datasheet/2/758/stepd-01-data-sheet-1143075.pdf
 */
#include "gpio.h"
#include "timer.h"

#define N_MOTOR_PINS 4

static const gpio_id_t motor_pins[N_MOTOR_PINS] = { GPIO_PB12, GPIO_PB11, GPIO_PB10, GPIO_PE17 };

enum { A = 1 << 0,
       B = 1 << 1,
       C = 1 << 2,
       D = 1 << 3,
};

static void step(bool clockwise) {
    static const int windings[] =  {A|B, B|C, C|D, D|A};
    int n = sizeof(windings)/sizeof(*windings);

    for (int phase = 0; phase < n; phase++) {
        int w = windings[clockwise ? phase : n - phase - 1];
        for (int i = 0; i < N_MOTOR_PINS; i++) {
            gpio_write(motor_pins[i], (w & (1 << i)) != 0);
        }
        timer_delay_ms(5);
    }
}

void main(void) {
    for (int i = 0; i < N_MOTOR_PINS; i++) {
        gpio_set_output(motor_pins[i]);
    }
    for (int i = 0; i < 25; i++) {
        step(true);
        timer_delay_ms(200);
    }
    for (int i = 0; i < 200; i++) {
        bool cw = i/50 % 2;
        step(cw);
    }
}
