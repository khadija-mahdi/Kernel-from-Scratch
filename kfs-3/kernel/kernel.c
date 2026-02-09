#include <drivers/vga/vga.h>
#include <kernel/memory/gdt.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/kmalloc.h>
#include <lib/lib.h>
// #include "vga.h"
// #include "../lib/gdt.h"
extern uint32_t _kernel_end;

typedef struct {
    uint32_t flags;
    uint32_t memLower;
    uint32_t memUpper;
    uint32_t bootDevice;
    uint32_t cmdLine;
    uint32_t modsCount;
    uint32_t modsAddr;
    uint32_t numSyms;
    uint32_t symSize;
    uint32_t symAddr;
    uint32_t mmapLength;
    uint32_t mmapAddr;
} multiboot_info_t;


void kernel_main(uint32_t magic,  multiboot_info_t *bootInfo)
{
    (void)magic; // Unused for no
    terminal_initialize(false);
    init_gdt();
    uint32_t modEnd = *(uint32_t *)(bootInfo->memUpper + 4); // Get upper memory size from multiboot info
    uint32_t physicalAllocStart = (modEnd * 1024) & ~0xFFF; // Start of physical memory after kernel
    initMemory(bootInfo->memUpper * 1024, physicalAllocStart); // Initialize memory management with upper memory size and physical allocation start
    // printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "Kernel initialized. Upper memory");
    kmallocInit(0x1000);
    
    
    keyboard_input();
    for(;;);
}