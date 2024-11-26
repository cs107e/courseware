EXTRAS = ../extras

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include -I$(EXTRAS) -Wall -ffreestanding -fstrict-volatile-bitfields
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango -lmango_gcc

all : playwav.bin

vpath %.c $(EXTRAS)
playwav.elf: i2s.o
playwav.c: welcome-to-machine.h

welcome-to-machine.h: welcome-to-machine.wav
	wav-to-c-header.py $< $@

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: playwav.bin
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~ welcome-to-machine.h

.PHONY: all clean
.PRECIOUS: %.o
