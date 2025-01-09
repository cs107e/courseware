APPLICATION = ps2_emulator.bin
INSTALL_DST=/Volumes/PS2EMULATOR

all: $(APPLICATION)

CFLAGS = -I$(RPI_CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T $(RPI_CS107E)/lib/memmap -L$(RPI_CS107E)/lib
LDLIBS 	= -lpi

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@
%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@
%.o: %.c ps2_emulator_constants.h
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

install: $(APPLICATION)
	cp $< $(INSTALL_DST)
	cp $< $(INSTALL_DST)/kernel.img

clean:
	rm -f *.bin *.elf *.list *.o *~

.PHONY: all clean install

ifndef RPI_CS107E
$(error ERROR: RPI_CS107E environment variable is not set)
endif
