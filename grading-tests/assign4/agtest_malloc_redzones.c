// Test whether whether writing into invalid memory generates a redzone corruption error

#include "grade_malloc.h"

void test_valid_write(void);
void test_write_before_block(void);
void test_write_after_block(void);

void run_test(void) {
    trace("Correct malloc/memset, expect free to succeed with no alert\n");
    for (int i = 0; i < 10; i++) {
        test_valid_write();
    }
    ref_printf("\n\n");
    test_write_before_block();
    ref_printf("\n\n");
    test_write_after_block();
}

void test_valid_write(void) {
    static unsigned int size;
    size += 8;
    char val = 0x28;

    char *ptr = malloc(size);
    memset(ptr, val, size);
    free(ptr);
}

void test_write_before_block(void) {
    unsigned int size = 23;
    char val = 0x35;

    trace("Testing write outside payload (BEFORE), expect valgrind alert\n");
    trace("Malloc block of size %d, memset contents to %x\n", size, val);
    char *ptr = malloc(size);
    memset(ptr, val, size);
    ptr[-1] = val;
    trace("Wrote to ptr[-1], call free should get valgrind alert\n");
    free(ptr);
}

void test_write_after_block(void) {
    unsigned int size = 11;
    char val = 0x71;

    trace("Testing write outside payload (AFTER), expect valgrind alert\n");
    trace("Malloc block of size %d, memset contents to %x\n", size, val);
    char *ptr = malloc(size);
    memset(ptr, val, size);
    ptr[size + 1] = val;
    trace("Wrote to ptr[size+1], call free should get valgrind alert\n");
    free(ptr);
}
