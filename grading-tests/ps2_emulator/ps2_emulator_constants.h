/* File:                ps2_emulator_constants.h
 * Author:              Peter McEvoy
 * Author:              Sean Konz
 * Author:              Julie Zelenski
 * Date last modified:  March 2023
 *
 * Constants used by the PS2 emulator and that should be publicly visible.
 */
#ifndef PS2_EMULATOR_CONSTANTS_H
#define PS2_EMULATOR_CONSTANTS_H

// Constants below need to be synchronized across driver and emulator.

#define PS2EM_HANDSHAKE_PULSE_USEC 30

// available sequence types, handshake sent by the driver to emulator selects which.
enum { PS2EM_CONSOLE_INPUT = 1, // not selected via handshake, long pulse on pin instead
       PS2EM_ALL = 2,
       PS2EM_INVALID_START = 3,
       PS2EM_INVALID_STOP_AND_PARITY = 4,
       PS2EM_INVALID_PARITY = 5,
       PS2EM_INVALID_STOP = 6,
};

// count of valid scancodes sent in sequence
// These counts must stay in synch with any changes in code!
static inline int ps2em_sequence_count(int which_sequence) {
    switch(which_sequence) {
        case PS2EM_ALL: return 255;
        case PS2EM_INVALID_START: return 2; // one valid code, one bad start bit, one valid code = 2 total
        case PS2EM_INVALID_STOP_AND_PARITY: return 3;
        case PS2EM_INVALID_PARITY: return 2;
        case PS2EM_INVALID_STOP: return 2;
        case PS2EM_CONSOLE_INPUT: return 107;
        default: return -1;
    }
}

static inline const char *ps2em_sequence_contents(int which_sequence) {
    switch(which_sequence) {
        case PS2EM_ALL: return "all PS2 scancodes";
        case PS2EM_INVALID_START: return "scancode with invalid start bit";
        case PS2EM_INVALID_STOP_AND_PARITY: return "scancode with invalid parity bit or invalid stop bit";
        case PS2EM_INVALID_PARITY: return "scancode with invalid parity bit";
        case PS2EM_INVALID_STOP: return "scancode with invalid stop bit";
        case PS2EM_CONSOLE_INPUT: return "scancode for console input";
        default: return "invalid sequence";
    }
}

#endif
