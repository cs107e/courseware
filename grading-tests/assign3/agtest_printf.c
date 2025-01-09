// Test printf mixed specifiers

#include "grade_printf.h"

void run_test(void) {
    printf("Hello, world!\n");
    printf("CS%d%c is my %s, in hex it's %x", 107, 'e', "favoritist", 0x107e);
}