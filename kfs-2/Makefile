# Makefile for KFS_1

# Tools
AS = nasm
CC = gcc
LD = ld

# Flags
ASFLAGS = -f elf32
CFLAGS = -g -m32 -ffreestanding -fno-builtin -fno-exceptions \
         -fno-stack-protector -nostdlib -nodefaultlibs \
         -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T linker.ld

# Output files
KERNEL = kernel.bin
ISO = kfs_1.iso

# Source files
ASM_SRC_DIRS = boot
ASM_SRC = $(foreach dir, $(ASM_SRC_DIRS), $(wildcard $(dir)/*.s))

C_SRC_DIRS = kernel lib
C_SRC = $(foreach dir, $(C_SRC_DIRS), $(wildcard $(dir)/*.c))


# Object files
ASM_OBJ = $(ASM_SRC:.s=.o)
C_OBJ = $(C_SRC:.c=.o)
OBJ = $(ASM_OBJ) $(C_OBJ)

# Default target
all: $(ISO)


# # Compile ASM files
boot/%.o: boot/%.s
	@echo "Assembling $<..."
	@$(AS) $(ASFLAGS) $< -o $@

# Generate per-directory compile rules for C_SRC_DIRS
define COMPILE_DIR_RULE
$(1)/%.o: $(1)/%.c
	@echo "Compiling $$<..."
	@$(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach dir,$(C_SRC_DIRS),$(eval $(call COMPILE_DIR_RULE,$(dir))))



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
	@qemu-system-i386  -cdrom $(ISO)

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