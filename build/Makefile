# Use system toolchain or your cross toolchain if you want
CC=gcc
LD=ld
AS=nasm

CFLAGS=-Os -s -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T linker.ld -nostdlib

SRC_DIR=../src
DIST_DIR=../dist

OBJS=$(DIST_DIR)/boot.o $(DIST_DIR)/kernel.o $(DIST_DIR)/lib_asm.o $(DIST_DIR)/lib_c.o

all: $(DIST_DIR)/kernel.elf

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

$(DIST_DIR)/boot.o: $(SRC_DIR)/boot.s | $(DIST_DIR)
	$(AS) -f elf32 $< -o $@

$(DIST_DIR)/lib_asm.o: $(SRC_DIR)/lib.asm | $(DIST_DIR)
	$(AS) -f elf32 $< -o $@

$(DIST_DIR)/lib_c.o: $(SRC_DIR)/lib.c | $(DIST_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIST_DIR)/kernel.o: $(SRC_DIR)/kernel.c | $(DIST_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIST_DIR)/kernel.elf: $(OBJS) linker.ld | $(DIST_DIR)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(DIST_DIR)
