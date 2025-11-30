
#include "vga.h"

void kernel_main(void)
{
    terminal_initialize();
    

    int i = 0;
    while (i < 50)
    {
        terminal_writestring("Line number: ");
        print_int(i);
        terminal_putchar('\n');
        i++;
    }
    terminal_writestring("Hello, Kernel World!\n");
    keyboard_input();
    while (1)
    {
    }
}