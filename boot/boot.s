; Multiboot constants
MBOOT_MAGIC     equ 0x1BADB002
MBOOT_FLAGS     equ 0x00000003
MBOOT_CHECKSUM  equ -(MBOOT_MAGIC + MBOOT_FLAGS)

; Multiboot header section (must be in first 8KB)
section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

; Reserve stack space (16KB)
section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

; Entry point
section .text
global _start
extern kernel_main

_start:
    ; Set up stack pointer
    mov esp, stack_top
    
    ; Call C kernel main function
    call kernel_main
    
    ; If kernel returns, halt the CPU
    cli
.hang:
    hlt
    jmp .hang