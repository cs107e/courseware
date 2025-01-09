THIS_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# use include to piggyback on student's existing makefile
# the - means don't complain if no Makefile found
# the error it would otherwise report is cryptic so
# below is target for Makefile with more clear message
-include Makefile
CFLAGS := -I../cs107e/include $(CFLAGS)
# per-assignment overrides after include of student makefile
-include $(THIS_DIR)/assign_overrides.make
LDFLAGS = $(ARCH) -nostdlib -L$$CS107E/lib -T $$CS107E/lib/memmap.ld
# search path to find libref_mango includes/lib
LIBREF_DIR = $(THIS_DIR)/../libref_mango/
CFLAGS += -I$(LIBREF_DIR)
LDFLAGS += -L$(LIBREF_DIR)
LDLIBS  += -lref_mango -lmango_gcc

# re-enable long-winded gcc messages for help diagnosing build errors
# ignore unused static functions
CFLAGS  += -fdiagnostics-show-option -Wno-unused-function

# vpath to find sources used as test code
vpath %.c $(THIS_DIR)
vpath %.s $(THIS_DIR)

Makefile:
	$(error No Makefile in $(PWD))

# assume every source file agtest_*.c is a test program
AGTEST_SOURCES := $(notdir $(wildcard $(THIS_DIR)/agtest_*.c))

# grading target makes all programs named agtest_*.bin
grading: $(AGTEST_SOURCES:.c=.bin)

symbols: $(AGTEST_SOURCES:.c=.symbols)

# symbols reports which symbols each .o depends on (i.e. student code)
%.symbols: %.o
	@riscv64-unknown-elf-nm -u $< |grep -v "U ref_" |grep -v "U __"

.INTERMEDIATE: $(AGTEST_SOURCES:.c=.o)

.PHONY:  grading %.symbols symbols
.PRECIOUS: %.elf
