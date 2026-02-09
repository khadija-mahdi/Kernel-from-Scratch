section .multiboot_header
align 4
header_start:
    MULTIBOOT_MAGIC equ 0x1BADB002
    MULTIBOOT_FLAGS equ 0x00000003  ; align modules (bit 0) + memory info (bit 1)
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)  ; checksum: magic + flags + checksum = 0
header_end:




SECTION .bss
ALIGN 16
stack_bottom:
    RESB 16384 * 8
stack_top:

section .boot

global _start
_start:
    MOV ecx, (initial_page_dir - 0xC0000000)
    MOV cr3, ecx

    MOV ecx, cr4
    OR ecx, 0x10
    MOV cr4, ecx

    MOV ecx, cr0
    OR ecx, 0x80000000
    MOV cr0, ecx

    JMP higher_half

section .text
higher_half:
    MOV esp, stack_top
    PUSH ebx
    PUSH eax
    XOR ebp, ebp
    extern kernel_main
    CALL kernel_main

halt:
    hlt
    JMP halt


section .data
align 4096
global initial_page_dir
initial_page_dir:
    DD 10000011b
    TIMES 768-1 DD 0

    DD (0 << 22) | 10000011b
    DD (1 << 22) | 10000011b
    DD (2 << 22) | 10000011b
    DD (3 << 22) | 10000011b
    TIMES 256-4 DD 0