#include "malloc.h"
#include "printf.h"
#include "uart.h"

extern void malloc_report(void);

void main(void) {
    uart_init();
    uart_putstring("\nCS107E_AUTO: START TEST\n");  // add markers for output scrape
    uart_putstring("CS107E_AUTO: main() has 10 mallocs and only 6 frees, should have 4 leaks remaining\n");
    void *ptr[10];

    ptr[0] = malloc(5);
    ptr[1] = malloc(128);
    ptr[2] = malloc(6);
    ptr[3] = malloc(24);
    free(ptr[3]);
    ptr[3] = malloc(16);
    free(ptr[1]);
    ptr[1] = malloc(11);
    ptr[4] = malloc(8);
    ptr[5] = malloc(16);
    free(ptr[5]);
    ptr[5] = malloc(21);
    free(ptr[4]);
    ptr[4] = malloc(59);
    free(ptr[0]);
    free(ptr[2]);
    printf("CS107E_AUTO: main() exited normally. Now expect automatic malloc report\n\n");
#if STAFF
    malloc_report();
#endif
}
