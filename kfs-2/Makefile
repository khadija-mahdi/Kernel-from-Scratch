# Makefile for KFS_1

# Tools
AS = nasm
CC = gcc
LD = ld

# Flags
ASFLAGS = -f elf32
CFLAGS = -g -I./include -m32 -ffreestanding -fno-builtin -fno-exceptions \
         -fno-stack-protector -nostdlib -nodefaultlibs \
         -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T linker.ld

# Directories
BUILD_DIR = build
SRC_DIRS = boot kernel lib drivers

# Output files
KERNEL = kernel.bin
ISO = kfs.iso

# Recursively find all source files
ASM_SRC = $(shell find $(SRC_DIRS) -name '*.s' 2>/dev/null)
C_SRC = $(shell find $(SRC_DIRS) -name '*.c' 2>/dev/null)

# Generate object file paths in build directory
ASM_OBJ = $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SRC))
C_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRC))
OBJ = $(ASM_OBJ) $(C_OBJ)

# Default target
all: $(ISO)

# Create build directory structure
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile ASM files
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "Assembling $<..."
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
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
	@cp boot/grub.cfg iso/boot/grub/
	@grub-mkrescue -o $(ISO) iso --compress=xz 2>/dev/null
	@rm -rf iso
	@echo "ISO created: $(ISO)"

# Run in QEMU
run: $(ISO)
	@echo "Starting QEMU..."
	@qemu-system-i386 -cdrom $(ISO)

# Debug: print variables
debug:
	@echo "ASM sources: $(ASM_SRC)"
	@echo "C sources: $(C_SRC)"
	@echo "ASM objects: $(ASM_OBJ)"
	@echo "C objects: $(C_OBJ)"

# Clean object files and kernel
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(KERNEL)
	@rm -rf iso

# Clean everything including ISO
fclean: clean
	@rm -f $(ISO)

# Rebuild everything
re: fclean all

.PHONY: all run clean fclean re debug














