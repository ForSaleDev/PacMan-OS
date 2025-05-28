# Makefile for Pacman OS

# Directories
SRC = src
BUILD = build
ISO = $(BUILD)/iso
BOOT = $(ISO)/boot
GRUB = $(BOOT)/grub
EFI = $(ISO)/EFI/BOOT

# Files
C_SOURCES = $(wildcard $(SRC)/*.c)
OBJS = $(C_SOURCES:.c=.o)
OBJS := $(patsubst $(SRC)/%.o,$(BUILD)/%.o,$(OBJS))

CC = x86_64-elf-gcc
LD = x86_64-elf-ld
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2
LDFLAGS = -T linker.ld -nostdlib -melf_x86_64

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
	mkdir -p $(BOOT)
	mkdir -p $(EFI)
	cp $(BUILD)/kernel.bin $(BOOT)/
	cp -r boot/grub/grub.cfg $(GRUB)/
	grub-mkstandalone -O x86_64-efi \
		-o $(EFI)/BOOTX64.EFI \
		"boot/grub/grub.cfg=boot/grub/grub.cfg"
	xorriso -as mkisofs \
		-R -J -joliet-long \
		-efi-boot EFI/BOOT/BOOTX64.EFI \
		-no-emul-boot -isohybrid-gpt-basdat \
		-o $(BUILD)/pacman.iso \
		$(ISO)

run: all
	qemu-system-x86_64 -cdrom $(BUILD)/pacman.iso -bios /usr/share/ovmf/OVMF.fd

clean:
	rm -rf $(BUILD)

.PHONY: all iso run clean