/* File: servo.c
 * -------------
 * SG90 MicroServo driven using PWM
 *
 * https://components101.com/motors/servo-motor-basics-pinout-datasheet
 */
#include "gpio.h"
#include "pwm.h"
#include "timer.h"


void main(void) {
    gpio_id_t pin = GPIO_PB1;
    pwm_channel_id_t ch = PWM4;

    pwm_init();
    pwm_config_channel(ch, pin, 50, false); // frequency 50 Hz
    while (1) {
        pwm_set_duty(ch, 1500*100/20000); // neutral
        timer_delay(1);
        pwm_set_duty(ch, 800*100/20000);  // CCW min
        timer_delay_ms(500);
        pwm_set_duty(ch, 2500*100/20000); // CW max
        timer_delay_ms(500);
    }
}
