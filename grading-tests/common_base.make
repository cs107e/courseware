# recursively invoke GRADING.make to build all agtest programs
# rest of makefile will be included into that one
# this makefile is standing in for student makefile
# idea is to build solution using same settings as student
# with libpi used for all module sources

all:
	make -f GRADING.make grading

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include $$warn $$freestanding -DSTAFF
CFLAGS += -fno-omit-frame-pointer -fstack-protector-strong
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango -lmango_gcc

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $< -o $@

clean:
	rm -f *.o *.bin *.elf *.list

# this rule will provide better error message when
# a source file cannot be found (missing, misnamed)
%.c:
	$(error cannot find source file `$@` needed for build)

.PHONY: all clean

# disable built-in rules (they are not used)
.SUFFIXES:

export warn = -Wall -Wpointer-arith -Wwrite-strings -Werror \
              -Wno-error=unused-function -Wno-error=unused-variable \
              -fno-diagnostics-show-option
export freestanding = -ffreestanding -nostdinc \
                      -isystem $(shell riscv64-unknown-elf-gcc -print-file-name=include)

ifndef CS107E
$(error CS107E environment variable is not set)
endif
