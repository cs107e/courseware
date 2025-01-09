# `make all` (or just `make)` compiles larson binary
all: larson.bin

# larson.bin is built from the assembly program larson.s
larson.bin: larson.s
	riscv64-unknown-elf-as larson.s -o larson.o
	riscv64-unknown-elf-objcopy larson.o -O binary larson.bin
	
# `make run` compiles larson binary and sends to the Pi to execute
run: larson.bin
	mango-run larson.bin

# `make clean` removes all compiled build products
clean:
	rm -f larson.o larson.bin
