# Makefile for Pacman OS

# Directories
SRC = src
BUILD = build
ISO = $(BUILD)/iso
BOOT = $(ISO)/boot
GRUB = $(BOOT)/grub

# Files
C_SOURCES = $(wildcard $(SRC)/*.c)
OBJS = $(C_SOURCES:.c=.o)
OBJS := $(patsubst $(SRC)/%.o,$(BUILD)/%.o,$(OBJS))

CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

all: $(BUILD)/kernel.bin iso

# Compile .c -> .o
$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link .o -> kernel.bin
$(BUILD)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Build bootable ISO
iso: $(BUILD)/kernel.bin
	mkdir -p $(GRUB)
	cp $(BUILD)/kernel.bin $(BOOT)/
	cp -r boot/grub/grub.cfg $(GRUB)/
	grub-mkrescue -o $(BUILD)/pacman.iso $(ISO) > /dev/null 2>&1

run: all
	qemu-system-i386 -cdrom $(BUILD)/pacman.iso

clean:
	rm -rf $(BUILD)