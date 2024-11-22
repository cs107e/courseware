#include "gpio.h"
#include "i2s.h"
#include "printf.h"
#include "uart.h"

#include "wav_files/wav.h"
#include "wav_files/welcome-to-machine.h"
// defines gwav

void audio_stream(int sample_freq, const int16_t data[], int n, bool is_mono) {
    int bits_per_sample = 16;
    i2s_init(sample_freq, bits_per_sample);
    i2s_config(is_mono ? MONO : STEREO, bits_per_sample);
    i2s_start();
    for (int i = 0; i < n; ) { // add all samples to stream
        if (is_mono) {
            i2s_write_fifo(data[i++]);
        } else { // stereo
            i2s_write_fifo(data[i++]);
            i2s_write_fifo(data[i++]);
        }
    }
    i2s_stop();
}

void main(void) {
    uart_init();
    audio_stream(gwav->frames_per_sec, gwav->data, gwav->n_data, gwav->samples_per_frame == 1);
}
