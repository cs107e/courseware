#include "gpio.h"
#include "interrupts.h"
#include "mouse.h"
#include "printf.h"
#include "uart.h"

static void test_mouse(void) {
    const char *actions[] = {"PRESS", "RELEASE", "MOVED", "DRAGGED"};

    printf("\nTry the mouse!\n");
    printf("Click both left and right together to exit\n");
    while (1) {
        mouse_event_t evt = mouse_read_event();
        printf("%7s {dx=%2d, dy=%2d} buttons [%d%d%d]%s\n",
                actions[evt.action],
                evt.dx, evt.dy,
                evt.left, evt.middle, evt.right,
                evt.x_overflow || evt.y_overflow ? "OVERFLOW!" : "");
        if (evt.left && evt.right) break;
    }
}

void main(void) {
    gpio_init();
    uart_init();
    interrupts_init();
    interrupts_global_enable(); // turn on before mouse_init

    printf("\nStarting %s\n", __FILE__);
    printf("\nAttempting to mouse_init...");
    mouse_init(MOUSE_CLOCK, MOUSE_DATA);
    printf("\nsuccessful.\n");
    interrupts_global_enable(); // after after mouse_init
    printf("Mouse initialized, read for mouse events.\n");
    test_mouse();
    printf("\nCompleted %s\n", __FILE__);
}
