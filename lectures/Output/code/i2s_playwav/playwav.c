/* File: playwav.c
 * --------------
 * PCM5102A Digital Audio I2S DAC board
 *
 */

#include "i2s.h"
#include "assert.h"
#include "printf.h"
#include "uart.h"

#include "welcome-to-machine.h" // header file defines gwav struct pointer

void main(void) {
    uart_init();

    printf("Playing wavfile '%s'\n", gwav->name);
    assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
    i2s_init(gwav->frames_per_sec);
    i2s_frame_type_t ftype = (gwav->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
    i2s_play_stream(gwav->frames_per_sec, gwav->data, gwav->n_data, ftype);
}
