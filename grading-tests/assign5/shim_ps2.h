#ifndef _SHIM_PS2_H
#define _SHIM_PS2_H
/* File:                shim_ps2.c
 * Author:              Peter McEvoy
 * Author:              Sean Konz
 * Date last modified:  2022 May juliez
 *
 * Shim implementation of the PS2 module whose ps2_read will deliver scancodes taken
 * from internally managed queue.
 */

#include "ps2.h"

static ps2_device_t *gThis = (void*)0x107e;

ps2_device_t *ps2_new(gpio_id_t clock_gpio, gpio_id_t data_gpio) {
     return gThis;
}

struct queue_entry {
    uint8_t scancode;
    const char *msg_prefix;
};

#define MAX_QUEUE_LENGTH 1000
static struct queue_entry gQueue[MAX_QUEUE_LENGTH];
static int gN_enqueued, gN_sent;

uint8_t ps2_read(ps2_device_t *dev) {
    if (dev != gThis) {
        trace("ps2_read called with invalid pointer to ps2 device");
    }
    while (gN_sent < gN_enqueued && gQueue[gN_sent].scancode == 0) gN_sent++; // skip embedded zeros
    if (gN_sent >= gN_enqueued) {   // Return zero if request when nothing left in queue
        return 0;
    } else {
        struct queue_entry cur = gQueue[gN_sent++];
        if (cur.msg_prefix) ref_printf("[%02x]\n", cur.scancode);
        else ref_printf("[%02x]\n", cur.scancode);
        return cur.scancode;
    }
}

bool ps2_is_empty(void) {
    return gN_sent >= gN_enqueued;
}

void ps2_enqueue_scancodes(struct queue_entry *scancodes, int n) {
    if (gN_enqueued + n > MAX_QUEUE_LENGTH)
        fatal_error("ps2 queue capacity exhausted");
    memcpy(gQueue + gN_enqueued, scancodes, n*sizeof(*scancodes));
    gN_enqueued += n;
}
#endif
