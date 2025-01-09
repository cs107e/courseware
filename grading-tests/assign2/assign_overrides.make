# Student makefile for assign2 does not yet link with library
# force that link here for grade tests
LDFLAGS = -nostdlib -L$$CS107E/lib -T $$CS107E/lib/memmap.ld
LDLIBS = -lmango -lmango_gcc
