#include "grade_keyboard.h"

void run_test(void) {

    static struct queue_entry scancodes[] = {
        // events without modifiers
        press_and_release(_C),
        press_and_release(_S),
        press_and_release(_1),
        press_and_release(_0),
        press_and_release(_7),
        press_and_release(_E),

        press(_ESC),
        press_and_release(_F5),
        release(_ESC),
        press_and_release(_BACKSPACE),

        // modifiers one at a time
        press(_SHIFT_LEFT),
            press_and_release(_A),
            release(_SHIFT_LEFT),
        press(_CTRL_LEFT),
            press_and_release(_B),
            release(_CTRL_LEFT),
        press(_ALT_LEFT),
            press_and_release(_C),
            release(_ALT_LEFT),

        // mixed/stacked modifiers
        press(_SHIFT_LEFT),
            press_and_release(_X),
            press(_CTRL_LEFT),
                press_and_release(_Y),
                press(EXT_ALT_RIGHT),
                    press(_Z),
                    release(_SHIFT_LEFT),
                press(_Z),
                release(EXT_ALT_RIGHT),
            press(_Z),
            release(_CTRL_LEFT),
        press_and_release(_1),
        release(_Z),

        press(_SHIFT_LEFT),
            press(_SHIFT_RIGHT),
                press_and_release(_R),
            release(_SHIFT_LEFT),
        release(_SHIFT_RIGHT),

        press(_CAPS_LOCK), // capslock down
            press_and_release(_G),
            release(_CAPS_LOCK), // capslock up
        press_and_release(_CAPS_LOCK), // capslock toggle

        // caps lock is sticky!
        press_and_release(_CAPS_LOCK), // capslock on
            press_and_release(_G),
            press_and_release(_CAPS_LOCK), // capslock off
        press_and_release(_3),

        // caps lock and shift
        press_and_release(_CAPS_LOCK), // capslock is on
            press(_SHIFT_LEFT),         // capslock + shift
                press_and_release(_Q),
                press_and_release(_CAPS_LOCK), // capslock off
                press_and_release(_R),
                press_and_release(_CAPS_LOCK), // capslock on
                release(_SHIFT_LEFT),   // shift off
            press_and_release(_S),
            press_and_release(_CAPS_LOCK), // capslock off

        press_and_release(_A) // end event sequence with non-modifier key event
    };

    ps2_enqueue_scancodes(scancodes, COUNT(scancodes));
    read_keyboard_events();
}
