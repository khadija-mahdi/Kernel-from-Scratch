#include <drivers/vga/vga.h>
#include <kernel/memory/gdt.h>
#include <drivers/keyboard/keyboard.h>

// #include "vga.h"
// #include "../lib/gdt.h"
extern uint32_t _kernel_end;

void kernel_main(void)
{
    terminal_initialize(false);
    init_gdt();
    uint32_t physicalAllocStart = ((uint32_t)&_kernel_end - 0xC0000000); // Convert to physical
    physicalAllocStart = (physicalAllocStart + 0xFFF) & ~0xFFF;         // Align to page
    // initMemory(0x1000000, physicalAllocStart); // Assume 16MB of RAM for now
    keyboard_input();
    while (1)
    {
    }
}