
#include "vga.h"

void kernel_main(void) {
    terminal_initialize();
    
    terminal_writestring("42");
    
    while (1) {
        __asm__ __volatile__("hlt");
    }
}