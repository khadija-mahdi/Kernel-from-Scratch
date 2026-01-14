
#include "vga.h"

void kernel_main(void)
{
    terminal_initialize();

    int keycode = 42;
    char *msg = "Kernel loaded\n";
    printk(msg);

    printk_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK, "Info: %s\n", "Kernel ready");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "Warning: %d free pages\n", keycode);
    printk_color(VGA_COLOR_RED, VGA_COLOR_BLACK, "Error: %s\n", "Disk not found");

    keyboard_input();
    while (1)
    {
    }
}