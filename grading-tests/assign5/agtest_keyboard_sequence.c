#include "grade_keyboard.h"

void run_test(void) {

    static struct queue_entry scancodes[] = {
        press(_P),
        press_and_release(_I),
        release(_P),
        press_and_release(_F11),
        press_and_release(_EQUAL),
        press_and_release(_SHIFT_LEFT),
        press_extended(EXT_ALT_RIGHT),
        press(_ENTER),
        release_extended(EXT_ALT_RIGHT),
        release(_ENTER),
        press_and_release_extended(EXT_CTRL_RIGHT),
        press_and_release(_SPACE),
    };

    ps2_enqueue_scancodes(scancodes, COUNT(scancodes));
    read_keyboard_sequences();
}
