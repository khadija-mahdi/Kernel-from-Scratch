# Makefile for KFS_1

# Tools
AS = nasm
CC = gcc
LD = ld

# Flags
ASFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-exceptions \
         -fno-stack-protector -nostdlib -nodefaultlibs \
         -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T linker.ld

# Output files
KERNEL = kernel.bin
ISO = kfs_1.iso

# Source files
ASM_SRC = boot/boot.s
C_SRC = kernel/kernel.c kernel/vga.c lib/keyboard.c lib/keyboadEntries.c lib/printk.c

# Object files
ASM_OBJ = $(ASM_SRC:.s=.o)
C_OBJ = $(C_SRC:.c=.o)
OBJ = $(ASM_OBJ) $(C_OBJ)

# Default target
all: $(ISO)

# Compile ASM files
boot/%.o: boot/%.s
	@echo "Assembling $<..."
	@$(AS) $(ASFLAGS) $< -o $@

# Compile C files
kernel/%.o: kernel/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL): $(OBJ)
	@echo "Linking kernel..."
	@$(LD) $(LDFLAGS) -o $@ $^
	@echo "Kernel binary created: $(KERNEL)"

# Create bootable ISO with GRUB
$(ISO): $(KERNEL)
	@echo "Creating ISO image..."
	@mkdir -p iso/boot/grub
	@cp $(KERNEL) iso/boot/
	@cp grub.cfg iso/boot/grub/
	@grub-mkrescue -o $(ISO) iso \
	--compress=xz 2>/dev/null
	@rm -rf iso
	@echo "ISO created: $(ISO)"

# Run in QEMU
run: $(ISO)
	@echo "Starting QEMU..."
	@qemu-system-i386 -cdrom $(ISO)

# Clean object files and kernel
clean:
	@echo "Cleaning..."
	@rm -f $(OBJ) $(KERNEL)
	@rm -rf iso

# Clean everything including ISO
fclean: clean
	@rm -f $(ISO)

# Rebuild everything
re: fclean all

.PHONY: all run clean fclean re