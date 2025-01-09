#include "grade_keyboard.h"

void run_test(void) {

    static struct queue_entry scancodes[] = {
        press(_A),
        press(_A),
        press(_A),
        release(_A),
        press_and_release(_F1),
        press_and_release(_F2),
        press_and_release(_F3),
        press_and_release(_F4),
        press_and_release(_F5),
        press_and_release(_F6),
        press_and_release(_F7),
        press_and_release(_F8),
        press_and_release(_F9),
        press_and_release(_F10),
        press_and_release(_F11),
        press_and_release(_F12),

        press_and_release(_BACK_TICK),
        press_and_release(_1),
        press_and_release(_2),
        press_and_release(_3),
        press_and_release(_4),
        press_and_release(_5),
        press_and_release(_6),
        press_and_release(_7),
        press_and_release(_8),
        press_and_release(_9),
        press_and_release(_0),
        press_and_release(_HYPHEN),
        press_and_release(_EQUAL),
        press_and_release(_BACKSPACE),

        press_and_release(_TAB),
        press_and_release(_Q),
        press_and_release(_W),
        press_and_release(_E),
        press_and_release(_R),
        press_and_release(_T),
        press_and_release(_Y),
        press_and_release(_U),
        press_and_release(_I),
        press_and_release(_O),
        press_and_release(_P),
        press_and_release(_SQUARE_BRACKET_LEFT),
        press_and_release(_SQUARE_BRACKET_RIGHT),
        press_and_release(_SLASH_BACK),

        // press_and_release(_CAPS_LOCK), // skip sticky modifier
        press_and_release(_A),
        press_and_release(_S),
        press_and_release(_D),
        press_and_release(_F),
        press_and_release(_G),
        press_and_release(_H),
        press_and_release(_J),
        press_and_release(_K),
        press_and_release(_L),
        press_and_release(_SEMICOLON),
        press_and_release(_APOSTROPHE),
        press_and_release(_ENTER),

        press_and_release(_Z),
        press_and_release(_X),
        press_and_release(_C),
        press_and_release(_V),
        press_and_release(_B),
        press_and_release(_N),
        press_and_release(_M),
        press_and_release(_COMMA),
        press_and_release(_PERIOD),
        press_and_release(_SLASH_FORWARD),

        press(_SPACE),

// now with modifiers in mix

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
