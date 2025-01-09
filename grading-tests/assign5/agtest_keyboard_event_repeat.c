#include "grade_keyboard.h"

void run_test(void) {

    static struct queue_entry scancodes[] = {

        press(_M), // simulate hold down M, repeat
        press(_M),
        press(_SHIFT_LEFT),
            release(_M),
            release(_SHIFT_LEFT),

        press(_SHIFT_LEFT), // simulate hold down Shift, repeat
        press(_SHIFT_LEFT),
        press(_SHIFT_LEFT),
        press(_SHIFT_LEFT),
            press(_A),
            press(_A),
            release(_SHIFT_LEFT),
        release(_A),

        press(_CAPS_LOCK), // simulate hold down CapsLock, repeat
        press(_CAPS_LOCK),
        press(_CAPS_LOCK),
        press(_CAPS_LOCK),
        release(_CAPS_LOCK),        // capslock on
            press_and_release(_Y),
        press_and_release(_CAPS_LOCK), // capslock off

        press_and_release(_Z) // end event sequence with non-modifier key event
    };

    ps2_enqueue_scancodes(scancodes, COUNT(scancodes));
    read_keyboard_events();
}
