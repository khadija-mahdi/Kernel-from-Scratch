#include "../kernel/vga.h"
#include "gdt.h"



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

void print_stack_trace()
{
    uint32_t esp = get_esp();
    uint32_t ebp = get_ebp();  // ← Get EBP register, don't read from ESP
    
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "-----------------------------------------------\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Stack Trace:\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Current ESP is at %x.\n", esp);
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Current EBP is at %x.\n", ebp);
    
    while (ebp != 0)
    {
        uint32_t return_address = *((uint32_t *)(ebp + 4));
        printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Return address: %x (from EBP: %x)\n", 
                     return_address, ebp);
        
        ebp = *((uint32_t *)ebp);  // Follow the chain
        
        // Safety check to prevent infinite loops
        if (ebp < 0x100000 || ebp > 0x4000)
            break;
    }
    
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "-----------------------------------------------\n");
}