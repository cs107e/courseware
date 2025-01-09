#ifndef GRADE_PS2_INTERRUPTS_H
#define GRADE_PS2_INTERRUPTS_H

#define DISABLE_NULL_WRITE_CHECK true
#include "../common_grading.h"
#include "../assign5/grade_ps2.h"
#include "ps2_ref.h"

static inline void read_ps2em_scancodes_with_interrupts(unsigned char which_sequence) {
    trace(VISUAL_BREAK);

#ifdef STAFF
    ref_interrupts_init(); // use ref modules for both ps2 and interrupts
    ps2_device_t *dev = ref_ps2_new(PS2EM_CLOCK, PS2EM_DATA);
    ref_ps2_use_interrupts(dev);
    ref_interrupts_global_enable();
#else
    interrupts_init();  // use same interrupts module as student (NOT ref version!)
    ps2_device_t *dev = ps2_new(PS2EM_CLOCK, PS2EM_DATA);
    interrupts_global_enable();
#endif
    int num_expected = send_ps2em_handshake(which_sequence);
    trace("expect to receive %d valid scancodes, will read via interrupt\n", num_expected);

    trace("timer_delay on main, ps2 device send events during sleep...\n");
    ref_timer_delay(1);
    ref_printf("\n");

    int num_received = 0;
    for (int i = 0; i < num_expected; i++) {
        uint8_t code = ps2_read(dev);
        ref_printf("%d: [%02x]\n", num_received++, code);
        if (code == 0) break;
        if (i < 5) ref_timer_delay_ms(200); // extra delay between first few
    }
    ref_printf("\n");
    trace("ps2_read(keyboard) read %d scancodes\n", num_received);
}


#endif