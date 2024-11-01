#include <stdint.h>
#include "printf.h"
#include "uart.h"

void bug_compare(void) {
    int a = -20;
    unsigned int b = 6;

    if( a < b )
        printf("-20 < 6 - all is well\n");
    else
        printf("-20 >= 6 - omg \n");
}

void overflow(void) {
    for (unsigned char i = 0; i < 256; i++)
        printf("0x%2x\n", i);
}

void conversions(void) {
    uint16_t before = 0xffff;
    uint32_t after = before;

    // uint32_t before = 0x12340001;
    // uint16_t after = before;

    // int16_t before = -1;
    // int32_t after = before;

    // int32_t before = -50000;
    // int16_t after = before;

    // int32_t before = -1;
    // uint32_t after = before;

    printf("BEFORE: %d (0x%x) -> AFTER: %d (0x%x) \n", before, before, after, after);
}

void main(void) {
    uart_init();

  //  overflow();
  //  bug_compare();
    conversions();
}
