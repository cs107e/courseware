#include "uart.h"
#include "printf.h"

                  // uninitialized
int gNum;
static int sgNum;

                       // initialized
int iNum = 1;
static int siNum = 2;

                       // const
const int cNum = 3;
static const int scNum = 4;


static void show_var(const int *p, const char *name) {
    printf("Variable %8s at address %p value %d\n", name, p, *p);
}

void main(void) {
    int local[4];

    uart_init();

    show_var(&gNum, "gNum");
    show_var(&sgNum, "sgNum");
    show_var(&iNum, "iNum");
    show_var(&siNum, "siNum");
    show_var(&cNum, "cNum");
    show_var(&scNum, "scNum");
    show_var(&local[0], "local[0]");
    show_var(&local[2], "local[2]");
}
