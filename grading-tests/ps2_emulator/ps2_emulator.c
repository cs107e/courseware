/* File:                ps2_emulator.c
 * Author:              Peter McEvoy
 * Author:              Sean Konz
 * Author:              Julie Zelenski
 * Date last modified:  March 2023
 *
 * Emulation of a PS2 device. Provides support for writing arbitrary scancodes.
 * The functionality provided is intentionally minimal as this component of the
 * grading infrastructure gave us many headaches over the course of our
 * four-quarter stint as CS107E CAs between winter 2020 and winter 2021.
 * Modify at your own risk.
 *
 * Updated Spring 2022 juliez
 * Add support for n-bit handshake via pulse, ack to confirm
 * Use separate pin from PS/2 clock/data to avoid unintended interference
 * Winter 2023 juliez
 * Added support for typed console input (for full system test)
 * trigger using single long pulse on handshake pin
 * Spring 2024 juliez
 * emulator running on RPi, driver on Mango Pi, rework handshake to be less timing sensitive
 */
#include "gpio.h"
#include "gpio_extra.h"
#include "pi.h"
#include "ps2_emulator_constants.h"
#include "timer.h"
#include "ps2_keys.h"

// connections on Rpi running PS2 emulator
#define PS2EM_CLOCK GPIO_PIN3
#define PS2EM_DATA GPIO_PIN4
#define PS2EM_HANDSHAKE_PI GPIO_PIN17

enum {
    PS2EM_NO_PULSE = 0,
    PS2EM_SHORT_PULSE = 1,
    PS2EM_LONG_PULSE = 2
};

static struct {
    unsigned int clock, data, handshake;
} emulator;

static void emulator_init(unsigned int clk, unsigned int data, unsigned int handshake) {
    emulator.clock = clk;
    gpio_set_output(emulator.clock);
    gpio_write(emulator.clock, 0);

    emulator.data = data;
    gpio_set_output(emulator.data);
    gpio_write(emulator.data, 1);

    emulator.handshake = handshake;
    gpio_set_input(emulator.handshake);
    gpio_enable_event_detection(emulator.handshake, GPIO_DETECT_RISING_EDGE);
}

static void emulator_write_bit(unsigned char bit) {
    // https://www.avrfreaks.net/sites/default/files/PS2%20Keyboard.pdf
    // clock must be high for 30-50us, then low for 30-50us
    #define US_PER_CLK_STATE 35

    gpio_write(emulator.clock, 1);  // clock should already be high, but ...

    // Change data in _middle_ of each cell
    timer_delay_us(US_PER_CLK_STATE/2);
    gpio_write(emulator.data, bit & 1);
    timer_delay_us(US_PER_CLK_STATE/2);

    // Bring clock low (signal now valid to read data)
    gpio_write(emulator.clock, 0);
    timer_delay_us(US_PER_CLK_STATE);

    // Return clock high
    gpio_write(emulator.clock, 1);
}

#define VALID_START_BIT 0
#define VALID_STOP_BIT 1
#define NUM_DATA_BITS 8

static void emulator_write_scancode(unsigned char scancode) {
    emulator_write_bit(VALID_START_BIT);
    unsigned char parity = 1;
    for (int i = 0; i < NUM_DATA_BITS; i++) {
        unsigned char bit = (scancode >> i) & 1;
        emulator_write_bit(bit);
        parity ^= bit;
    }
    emulator_write_bit(parity);
    emulator_write_bit(VALID_STOP_BIT);

    // delay to let driver catch up
    #define US_AFTER_SCANCODE (US_PER_CLK_STATE*100)
    timer_delay_us(US_AFTER_SCANCODE);
}

static void emulator_write_corrupt_start(void) {
    emulator_write_bit(!VALID_START_BIT);   // invalid start
}

static void emulator_write_corrupt_parity(void) {
    emulator_write_bit(VALID_START_BIT);
    for (int i = 0; i < NUM_DATA_BITS; i++) {
        emulator_write_bit(0);    // all data bits 0
    }
    emulator_write_bit(0); // invalid parity
}

static void emulator_write_corrupt_stop(void) {
    emulator_write_bit(VALID_START_BIT);
    for (int i = 0; i < NUM_DATA_BITS; i++) {
        emulator_write_bit(0);    // all data bits 0
    }
    emulator_write_bit(1);    // valid parity bit
    emulator_write_bit(!VALID_STOP_BIT);    // invalid stop
}

// driver selects which sequence to send by pulsing
// handshake pin 1 to N times, delay between pulses
// must be < 1000us
static int wait_for_pulse(int wait_usec) {
    unsigned int start_ticks = timer_get_ticks();
    bool led_state = true;
    while (!gpio_check_and_clear_event(emulator.handshake)) {
        unsigned int waited = timer_get_ticks() - start_ticks;
        if (wait_usec > 0 && waited > wait_usec) return PS2EM_NO_PULSE;
        int seconds_waited = waited/1000000;
        if ((seconds_waited & 1) == led_state) { // slow toggle green LED every sec
            led_state ^= 1;
            pi_led_toggle(PI_ACT_LED);
        }
    }
    start_ticks = timer_get_ticks();
    while (gpio_read(emulator.handshake) == 1) {}  // wait for pin release
    unsigned int waited = timer_get_ticks() - start_ticks;
    if (waited < PS2EM_HANDSHAKE_PULSE_USEC*10) return PS2EM_SHORT_PULSE;
    else return PS2EM_LONG_PULSE;
}

static int wait_for_handshake(void) {
    int which_sequence = -1;

    pi_led_on(PI_ACT_LED);  // both on start of wait for handshake
    pi_led_on(PI_PWR_LED);
    gpio_write(emulator.clock, 0); // clock low, data high state tells driver waiting on handshake
    gpio_write(emulator.data, 1);
    gpio_check_and_clear_event(emulator.handshake); // discard any previous event
    int pulse = wait_for_pulse(-1); // wait indefinitely for first pulse
    if (pulse == PS2EM_LONG_PULSE) {
        return PS2EM_CONSOLE_INPUT;
    } else {
        int pulse_count = 1;
        while (wait_for_pulse(1000)) { // at most delay 1000us between pulse, if longer assume end
            pulse_count++;
        }
        which_sequence = pulse_count;
    }

    int num = ps2em_sequence_count(which_sequence); // confirm this corresponds to valid sequence
    if (num == -1) { pi_abort(); }

    gpio_write(emulator.clock, 1); // signal driver that emulator now ready to send
    for (int i = 0; i < which_sequence; i++) { // blink both LED to confirm which sequence
        pi_led_on(PI_PWR_LED);
        pi_led_on(PI_ACT_LED);
        timer_delay_ms(200);
        pi_led_off(PI_PWR_LED);
        pi_led_off(PI_ACT_LED);
        timer_delay_ms(100);
    }
    return which_sequence;
}

void emulator_type_input(const char *str) {
    for (int i = 0; str[i]; i++) {
        char ch = str[i];
        if (ch == '\n') timer_delay_ms(1000); // delay at end of line
        for (int code = 0; code < 0x83; code++) {
            if (ps2_keys[code].ch == ch) {
                emulator_write_scancode(code);
                emulator_write_scancode(0xf0);
                emulator_write_scancode(code);
                break;
            } else if (ps2_keys[code].other_ch == ch) {
                emulator_write_scancode(0x59);
                emulator_write_scancode(code);
                emulator_write_scancode(0xf0);
                emulator_write_scancode(code);
                emulator_write_scancode(0xf0);
                emulator_write_scancode(0x59);
                break;
            }
        }
    }
}

void main(void) {

    while (1) { // emulator will run indefinitely, if you need to reset, do it physically

        emulator_init(PS2EM_CLOCK, PS2EM_DATA, PS2EM_HANDSHAKE_PI);
        unsigned char which_sequence = wait_for_handshake(); // sit idle until driver request via handshake pin

        pi_led_on(PI_ACT_LED);  // both LED on solid while sending sequence
        pi_led_on(PI_PWR_LED);

        switch (which_sequence) {
            case PS2EM_ALL: // send all possible 8-bit scancode, zero last
                for (int scancode = 0xff; scancode >= 0; scancode--) {
                    emulator_write_scancode(scancode);
                }
                break;

            case PS2EM_INVALID_START:  // corrupt start bit in between valid scancodes
                emulator_write_scancode(0x20);
                emulator_write_corrupt_start();
                emulator_write_scancode(0x21);
                break;

            case PS2EM_INVALID_STOP_AND_PARITY: // corrupt parity and stop bits in between valid scancodes
                emulator_write_scancode(0x30);
                emulator_write_corrupt_parity();
                emulator_write_scancode(0x31);
                emulator_write_corrupt_stop();
                emulator_write_scancode(0x32);
                break;

            case PS2EM_INVALID_PARITY:  // corrupt parity bit in between valid scancodes
                emulator_write_scancode(0x40);
                emulator_write_corrupt_parity();
                emulator_write_scancode(0x41);
                break;

            case PS2EM_INVALID_STOP: // corrupt stop bit in between valid scancodes
                emulator_write_scancode(0x50);
                emulator_write_corrupt_stop();
                emulator_write_scancode(0x51);
                break;

            case PS2EM_CONSOLE_INPUT:
                {
                 static const char *input =
                //123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789
                "echo hello, world!\n"
                "nonsense command with many args, wrap at right edge & truncate shell_read BEEP!!\n"
                "help\n"
                "help echo\n"
                "peek 0x18\n"
                "poke 0x40000000 0x1234\n"
                "peek 0x40000000\n"
                "echo CS107e is the best\b\b\b\bBEST and I'm DONE!\n"
                "reboot\n";
                emulator_type_input(input);
                continue;   // don't add extra zeros in this case
                }

            default:
                pi_abort();
        }
        for (int i = 0; i < 3; i++)
            emulator_write_scancode(0); // end all sequences with extra zeros as backstop

        pi_led_off(PI_ACT_LED);  // both off at end of sequence
        pi_led_off(PI_PWR_LED);
    }
}
