all: list

OBJDUMP_FLAGS = -d

%.riscv.o: %.c
	riscv64-unknown-elf-gcc -O2 -march=rv64im -mabi=lp64 -c $< -o $@

%.intel.o: %.c
	clang -O2 -fomit-frame-pointer -c $< -o $@

%.arm.o: %.c
	arm-none-eabi-gcc -O2 -c $< -o $@

# Extra recipe to disassemble the compiled program
list: code.riscv.o code.intel.o code.arm.o
	arm-none-eabi-objdump $(OBJDUMP_FLAGS) -d code.arm.o >arm.list
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d code.riscv.o >riscv.list
	otool -v -t -j code.intel.o >intel.list

clean:
	rm -rf *.o *.bin *.elf *.list *~ code

.PHONY: all clean run list
.PRECIOUS:  %.elf
