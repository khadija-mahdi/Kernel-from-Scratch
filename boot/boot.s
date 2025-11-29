section .multiboot_header
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    
    ; checksum
    dd -(0xe85250d6 + 0 + (header_end - header_start))
    
    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:


section .note.GNU-stack noalloc noexec nowrite progbits

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