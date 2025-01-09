#ifndef GRADE_KEYBOARD_H
#define GRADE_KEYBOARD_H

#include "../common_grading.h"
#include "shim_ps2.h"

static bool isprint(char ch) { return ch >= ' ' && ch <= '~'; }
static bool isfunction(char ch) { return ch >= PS2_KEY_F1 && ch <= PS2_KEY_F12; }

static inline void ref_print_ps2key(char ch) {
    if (isprint(ch)) {
        ref_printf("%c", ch);
    } else {
        if (isfunction(ch)) {
            ref_printf("F%d", ch - PS2_KEY_F1 + 1);
        } else {
            ref_printf("[%02x]", ch);
        }
    }
}

static inline void read_keyboard_sequences(void) {
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

    while (!ps2_is_empty()) {
        key_action_t sequence = keyboard_read_sequence();
        trace("keyboard_read_sequence: { keycode = %02x, action = %s }\n", sequence.keycode,
            sequence.what == KEY_PRESS ? "press" : "release");
    }
}

static inline void read_keyboard_events(void) {
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

    while (!ps2_is_empty()) {
        key_event_t event = keyboard_read_event();
        trace("keyboard_read_event: { action = %7s, modifiers = 0x%x, ps2key = ",
                event.action.what == KEY_PRESS ? "press" : "release", event.modifiers);
        ref_print_ps2key(event.key.ch);
        ref_printf("/");
        ref_print_ps2key(event.key.other_ch);
        ref_printf(" }\n");
    }
}

static inline void read_keyboard_chars(void) {
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

    while (!ps2_is_empty()) {
        char ch = keyboard_read_next();
        trace("keyboard_read_next: ");
        ref_print_ps2key(ch);
        ref_printf("\n");
    }
}

// Scancode prefixed to an extended key.
#define EXTENDED_SCANCODE 0xE0

// Scancode prefixed to a key release.
#define RELEASE_SCANCODE  0xF0

// Take action on a non-extended key. Use with scancode macros beginning with `_`.
#define press(scancode) \
    {scancode, ""}

#define release(scancode)      \
    {RELEASE_SCANCODE, "f0 "}, \
    {scancode, NULL}

#define press_and_release(scancode) \
    press(scancode),                \
    release(scancode)

// Take action on an extended key. Use with scancode macros beginning with `EXT_`.
#define press_extended(scancode)  \
    {EXTENDED_SCANCODE, "e0 "},   \
    {scancode, NULL}

#define release_extended(scancode) \
    {EXTENDED_SCANCODE, "e0 f0 "}, \
    {RELEASE_SCANCODE, NULL},      \
    {scancode, NULL}

#define press_and_release_extended(scancode) \
    press_extended(scancode),                \
    release_extended(scancode)

// Scancode macros.
// See https://techdocs.altium.com/display/FPGA/PS2+Keyboard+Scan+Codes.
#define _ESC                  0x76
#define _F1                   0x05
#define _F2                   0x06
#define _F3                   0x04
#define _F4                   0x0C
#define _F5                   0x03
#define _F6                   0x0B
#define _F7                   0x83
#define _F8                   0x0A
#define _F9                   0x01
#define _F10                  0x09
#define _F11                  0x78
#define _F12                  0x07
#define _SCROLL_LOCK          0x7E
#define _BACK_TICK            0x0E
#define _1                    0x16
#define _2                    0x1E
#define _3                    0x26
#define _4                    0x25
#define _5                    0x2E
#define _6                    0x36
#define _7                    0x3D
#define _8                    0x3E
#define _9                    0x46
#define _0                    0x45
#define _HYPHEN               0x4E
#define _EQUAL                0x55
#define _BACKSPACE            0x66
#define _TAB                  0x0D
#define _Q                    0x15
#define _W                    0x1D
#define _E                    0x24
#define _R                    0x2D
#define _T                    0x2C
#define _Y                    0x35
#define _U                    0x3C
#define _I                    0x43
#define _O                    0x44
#define _P                    0x4D
#define _SQUARE_BRACKET_LEFT  0x54
#define _SQUARE_BRACKET_RIGHT 0x5B
#define _SLASH_BACK           0x5D
#define _CAPS_LOCK            0x58
#define _A                    0x1C
#define _S                    0x1B
#define _D                    0x23
#define _F                    0x2B
#define _G                    0x34
#define _H                    0x33
#define _J                    0x3B
#define _K                    0x42
#define _L                    0x4B
#define _SEMICOLON            0x4C
#define _APOSTROPHE           0x52
#define _ENTER                0x5A
#define _SHIFT_LEFT           0x12
#define _Z                    0x1A
#define _X                    0x22
#define _C                    0x21
#define _V                    0x2A
#define _B                    0x32
#define _N                    0x31
#define _M                    0x3A
#define _COMMA                0x41
#define _PERIOD               0x49
#define _SLASH_FORWARD        0x4A
#define _SHIFT_RIGHT          0x59
#define _CTRL_LEFT            0x14
#define EXT_WINDOWS_LEFT      0x1F
#define _ALT_LEFT             0x11
#define _SPACE                0x29
#define EXT_ALT_RIGHT         0x11
#define EXT_WINDOWS_RIGHT     0x27
#define EXT_MENUS             0x2F
#define EXT_CTRL_RIGHT        0x14
#define EXT_INSERT            0x70
#define EXT_HOME              0x6C
#define EXT_PAGE_UP           0x7D
#define EXT_DELETE            0x71
#define EXT_END               0x69
#define EXT_PAGE_DOWN         0x7A
#define EXT_ARROW_UP          0x75
#define EXT_ARROW_LEFT        0x6B
#define EXT_ARROW_DOWN        0x72
#define EXT_ARROW_RIGHT       0x74
#define _NUMPAD_NUM_LOCK      0x77
#define EXT_NUMPAD_DIVIDE     0x4A
#define _NUMPAD_MULTIPLY      0x7C
#define _NUMPAD_SUBTRACT      0x7B
#define _NUMPAD_7             0x6C
#define _NUMPAD_8             0x75
#define _NUMPAD_9             0x7D
#define _NUMPAD_ADD           0x79
#define _NUMPAD_4             0x6B
#define _NUMPAD_5             0x73
#define _NUMPAD_6             0x74
#define _NUMPAD_1             0x69
#define _NUMPAD_2             0x72
#define _NUMPAD_3             0x7A
#define _NUMPAD_0             0x70
#define _NUMPAD_DECIMAL       0x71
#define EXT_NUMPAD_ENTER      0x5A

uint8_t rows[6][14] = {
    { _ESC, _F1, _F2, _F3, _F4, _F5, _F6, _F7, _F8, _F9, _F10, _F11, _F12 },
    { _BACK_TICK, _1, _2, _3, _4, _5, _6, _7, _8, _9, _0, _HYPHEN, _EQUAL, _BACKSPACE },
    { _TAB, _Q, _W, _E, _R, _T, _Y, _U, _I, _O, _P, _SQUARE_BRACKET_LEFT, _SQUARE_BRACKET_RIGHT, _SLASH_BACK },
    { _CAPS_LOCK, _A, _S, _D, _F, _G, _H, _J, _K, _L, _SEMICOLON, _APOSTROPHE, _ENTER },
    { _SHIFT_LEFT, _Z, _X, _C, _V, _B, _N, _M, _COMMA, _PERIOD, _SLASH_FORWARD, _SHIFT_RIGHT },
    { _ALT_LEFT, EXT_WINDOWS_LEFT,_CTRL_LEFT, _SPACE, EXT_ALT_RIGHT, EXT_WINDOWS_RIGHT, EXT_CTRL_RIGHT },
};
uint8_t digit_keys[] = {
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _0
};
uint8_t alphabet_keys[] = {
    _A, _B, _C, _D, _E, _F, _G, _H, _I, _J, _K, _L, _M, _N, _O, _P, _Q, _R, _S, _T, _U, _V, _W, _X, _Y, _Z
};
uint8_t modifier_keys[] = {
    _SHIFT_LEFT, _SHIFT_RIGHT, _CTRL_LEFT, EXT_CTRL_RIGHT, _ALT_LEFT, EXT_ALT_RIGHT, EXT_WINDOWS_LEFT, EXT_WINDOWS_RIGHT
};
uint8_t numpad_keys[] = {
    _NUMPAD_1, _NUMPAD_2, _NUMPAD_3, _NUMPAD_4, _NUMPAD_5, _NUMPAD_6, _NUMPAD_7, _NUMPAD_8, _NUMPAD_9, _NUMPAD_0,
    _NUMPAD_DECIMAL, EXT_NUMPAD_ENTER, _NUMPAD_ADD, _NUMPAD_SUBTRACT, _NUMPAD_MULTIPLY, _NUMPAD_DECIMAL, _NUMPAD_NUM_LOCK
};
uint8_t extra_keys[] = {
    EXT_ARROW_UP, EXT_ARROW_DOWN, EXT_ARROW_LEFT, EXT_ARROW_RIGHT,
    EXT_INSERT, EXT_DELETE, EXT_HOME, EXT_END, EXT_PAGE_DOWN, EXT_PAGE_UP
};


#endif
