#include <lib/lib.h>
#include <drivers/vga/vga.h>
#include <kernel/memory/gdt.h>



// #include <kernel/vga.h>
// // #include "../kernel/vga.h"
// #include "gdt.h"



// extern void gdt_flush(uint32_t);

uint32_t get_esp()
{
    uint32_t esp;
    __asm__ __volatile__ (
        "movl %%esp, %0"
        : "=r" (esp)
    );
    return esp;
}

uint32_t get_ebp()  // ← Add this function
{
    uint32_t ebp;
    __asm__ __volatile__ (
        "movl %%ebp, %0"
        : "=r" (ebp)
    );
    return ebp;
}
