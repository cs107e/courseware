#include "grade_ps2.h"
#include "../assign7/grade_ps2_interrupts.h"
#include "console_ref.h"


void shell_autoinput(void) {
   ref_keyboard_init(PS2EM_CLOCK, PS2EM_DATA);
   ref_shell_init(ref_keyboard_read_next, ref_printf);
   send_ps2em_handshake(PS2EM_CONSOLE_INPUT);
   ref_shell_run();  // will not return
}

void console_autoinput(void) {
   ref_interrupts_init();
   ref_keyboard_init(PS2EM_CLOCK, PS2EM_DATA);
   ref_keyboard_use_interrupts();
   ref_console_init(25, 80, GL_BLACK, GL_WHITE);
   ref_shell_init(ref_keyboard_read_next, ref_console_printf);
   interrupts_global_enable();
   send_ps2em_handshake(PS2EM_CONSOLE_INPUT);
   ref_shell_run();  // will not return
}

// Use this test driver to locally confirm operation of ps2 emulator
void run_test(void) {
   trace("This test program prompts the emulator to send available sequences\n");

   read_ps2em_scancodes(PS2EM_ALL);

   read_ps2em_scancodes(PS2EM_INVALID_START);

   read_ps2em_scancodes(PS2EM_INVALID_STOP_AND_PARITY);

   read_ps2em_scancodes(PS2EM_INVALID_PARITY);

   read_ps2em_scancodes(PS2EM_INVALID_STOP);

   //shell_autoinput();
   //console_autoinput();

   // trace("Repeat ps2 all sequence, read via interrupt\n");
   // read_ps2em_scancodes_with_interrupts(PS2EM_ALL);

   // trace("Request invalid sequence number, expect assert/error\n");
   // read_ps2em_scancodes(12); // invalid request
}
