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
    (void)magic;
    terminal_initialize(false);
    init_gdt();
    // Compute physical end of kernel from the linker symbol, aligned to 4KB
    uint32_t physicalAllocStart = (((uint32_t)&_kernel_end - 0xC0000000) + 0xFFF) & ~0xFFF;
    initMemory(bootInfo->memUpper * 1024, physicalAllocStart);
    kmallocInit(0x1000);
    printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "Kernel initialized. Upper memory");
    

    keyboard_input();
    
    for(;;);
}