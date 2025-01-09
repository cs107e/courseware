#ifndef GRADE_PS2_H
#define GRADE_PS2_H

#include "../common_grading.h"
#include "../ps2_emulator/ps2_emulator_constants.h"
#include "gpio_ref.h"
#include "ps2.h"

#define PS2EM_CLOCK GPIO_PD12
#define PS2EM_DATA GPIO_PD13
#define PS2EM_HANDSHAKE GPIO_PD11

#define ONE_SEC 1000000

static bool wait_for_clock_high(unsigned long timeout_usecs) {
    unsigned int start_ticks = ref_timer_get_ticks();
    unsigned int too_long = start_ticks + timeout_usecs*TICKS_PER_USEC;
    while (ref_gpio_read(PS2EM_CLOCK) != 1)  {
        if (ref_timer_get_ticks() > too_long) return false;
    }
    return true;
}

static inline int send_ps2em_handshake(unsigned char which_sequence) {
    const int handshake_pin = PS2EM_HANDSHAKE;
    int num_scancodes = ps2em_sequence_count(which_sequence);
    if (num_scancodes == -1) {
        fatal_error("invalid scancode sequence requested");
    }
    // emulator signals ready for handshake when clock low and data high
    if (ref_gpio_read(PS2EM_CLOCK) != 0 || ref_gpio_read(PS2EM_DATA) != 1) {
        fatal_error("unable to initiate handshake with ps2 emulator, clock and data state not valid");
    }
    trace("Request sequence #%d: %s\n", which_sequence, ps2em_sequence_contents(which_sequence));
    ref_gpio_set_output(handshake_pin);
    for (int i = 0; i < which_sequence; i++) {
        // rising edge on handshake_pin will poke emulator
        ref_gpio_write(handshake_pin, 0);
        ref_gpio_write(handshake_pin, 1);
        ref_timer_delay_us(which_sequence == PS2EM_CONSOLE_INPUT? ONE_SEC: PS2EM_HANDSHAKE_PULSE_USEC);
        ref_gpio_write(handshake_pin, 0);
    }

    // emulator signals ready to send sequence by raising clock high
    if (!wait_for_clock_high(ONE_SEC)) {
        fatal_error("unable to complete handshake with ps2 emulator");
    }
    return num_scancodes;
}

static inline void read_ps2em_scancodes(unsigned char which_sequence) {
    trace(VISUAL_BREAK);
    ps2_device_t *dev = ps2_new(PS2EM_CLOCK, PS2EM_DATA); // call student init of ps2 device first
    int num_expected = send_ps2em_handshake(which_sequence);
    trace("expect to receive %d valid scancodes...\n", num_expected);

    // Reading from ps2 emulator which is sending codes rapid fire
    // First is tight loop to read all codes into array, only after
    // verify what was read and output codes
    uint8_t scancodes[num_expected];
    int num_received = 0;
    for (int i = 0; i < num_expected; i++) {
        uint8_t code = ps2_read(dev);
        scancodes[num_received++] = code;
        if (code == 0) break;
    }
    trace("ps2_read(keyboard) read %d scancodes\n", num_received);
    ref_printf("\n");
    for (int i = 0; i < num_received; i++) {
        ref_printf("%d: [%02x]\n", i, scancodes[i]);
    }
}

#endif
