
#include "vga.h"

void kernel_main(void) {
    terminal_initialize();
    
    terminal_writestring("42");
    
    while (1) {
    }
}