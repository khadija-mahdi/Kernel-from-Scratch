#include <drivers/vga/vga.h>
#include <kernel/memory/gdt.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/kmalloc.h>
#include <kernel/memory/vmalloc.h>
#include <kernel/memory/panic.h>
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
    printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "Kernel initialized. Testing memory subsystems...\n\n");
   // Allocate and use like normal malloc
    char *str = (char *)kmalloc(20);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = '\0';
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "String: %s\n", str);

    // Allocate an integer
    int *num = (int *)kmalloc(sizeof(int));
    *num = 42;
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Number: %d\n", *num);

    // Allocate an array
    int *arr = (int *)kmalloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++)
        arr[i] = i * 10;
    for (int i = 0; i < 5; i++)
        printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "arr[%d] = %d\n", i, arr[i]);

    // Allocate a struct
    typedef struct {
        int x;
        int y;
    } Point;

    Point *p = (Point *)kmalloc(sizeof(Point));
    p->x = 100;
    p->y = 200;
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Point: (%d, %d)\n", p->x, p->y);
    printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "Addresses: str=%x, num=%x, arr=%x, p=%x\n", (uint32_t)str, (uint32_t)num, (uint32_t)arr, (uint32_t)p);

    // kfree(str);
    // kfree(num);
    // kfree(arr);
    // kfree(p);
    
    check_stack_overflow();
    keyboard_input();
    
    for(;;);
}