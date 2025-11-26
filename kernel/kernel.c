/* kernel/kernel.c - Kernel entry point */

#include "vga.h"

/* This is the entry point of our kernel */
void kernel_main(void) {
    /* Initialize the terminal */
    terminal_initialize();
    
    /* Display "42" on the screen */
    terminal_writestring("Hello World!");
    
    /* Infinite loop - kernel should never exit */
    while (1) {
        /* Halt CPU until next interrupt */
        __asm__ __volatile__("hlt");
    }
}(start)

SECTIONS {
    . = 1M;

    .boot :
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }

    .text :
    {
        *(.text)
    }
}