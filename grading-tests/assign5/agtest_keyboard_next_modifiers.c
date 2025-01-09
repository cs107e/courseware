#include "grade_keyboard.h"

void run_test(void) {

    static struct queue_entry scancodes[] = {

        press(_SHIFT_LEFT),
            press_and_release(_A),
            press_and_release(_Z),
            press_and_release(_0),
            press_and_release(_HYPHEN),
            press_and_release(_EQUAL),
        release(_SHIFT_LEFT),

        press_and_release(_CAPS_LOCK),
            press_and_release(_J),
            press_and_release(_K),
            press_and_release(_4),
            press_and_release(_COMMA),
            press_and_release(_PERIOD),
        press_and_release(_CAPS_LOCK),

        press(_SHIFT_RIGHT),
            press_and_release(_P),
            press_and_release(_Q),
            press_and_release(_7),
            press_and_release(_BACK_TICK),
            press_and_release(_SLASH_FORWARD),
        release(_SHIFT_RIGHT),

        press_and_release(_CAPS_LOCK),
            press(_SHIFT_LEFT),
                press_and_release(_V),
                press_and_release(_X),
                press_and_release(_2),
                press_and_release(_BACK_TICK),
                press_and_release(_SEMICOLON),
            release(_SHIFT_LEFT),
            press_and_release(_Y),
            press_and_release(_Z),
            press_and_release(_3),
            press_and_release(_APOSTROPHE),

        press(_E)
    };

    ps2_enqueue_scancodes(scancodes, COUNT(scancodes));
    read_keyboard_chars();
}
