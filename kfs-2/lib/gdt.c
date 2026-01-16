#include "gdt.h"
#include "../kernel/vga.h"

extern void gdt_flush(uint32_t);

// Declare the actual array at the special section
struct gdt_entry_struct gdt_entries[7] __attribute__((section(".gdt")));
struct gdt_ptr_struct gdt_ptr;

void init_gdt()
{
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 7) - 1;
    gdt_ptr.base = (uint32_t)gdt_entries; // Use the actual array address
    
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Stack
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code
    gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data
    gdt_set_gate(6, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User stack
    
    // printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "GDT: Setting up GDT entries done.\n");
    // printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "GDT: is at %x.\n", (uint32_t)gdt_entries);
    
    gdt_flush((uint32_t)&gdt_ptr);
}

void gdt_set_gate(int num, uint32_t base, uint32_t limit,
                  uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;
    gdt_entries[num].limit       = (limit & 0xFFFF);
    gdt_entries[num].flags       = (limit >> 16) & 0x0F;
    gdt_entries[num].flags      |= gran & 0xF0;
    gdt_entries[num].access      = access;
}









// #include "gdt.h"
// #include "../kernel/vga.h"

// extern void gdt_flush(uint32_t);

// // In gdt.c

// struct gdt_entry_struct gdt_entries[7] __attribute__((section(".gdt")));
// struct gdt_ptr_struct gdt_ptr;

// void init_gdt()
// {
//     gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 7) - 1;
//     gdt_ptr.base = GDT_ADDR; // Address of gdt_entries

//     gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
//     gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code
//     gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data
//     gdt_set_gate(3, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Stack

//     gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code
//     gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data
//     gdt_set_gate(6, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User stack
//     printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "GDT: Setting up GDT entries done.\n");
//     printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "GDT: is %x.\n", GDT_ADDR);
//     gdt_flush((uint32_t)&gdt_ptr);
// }

// void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
// {

//     gdt_entries[num].base_low = (base & 0xFFFF);
//     printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "GDT: is %x.\n", gdt_entries[num].base_low);
//     gdt_entries[num].base_middle = (base >> 16) & 0xFF;
//     gdt_entries[num].base_high = (base >> 24) & 0xFF;

//     gdt_entries[num].limit = (limit & 0xFFFF);
//     gdt_entries[num].flags = (limit >> 16) & 0x0F;

//     gdt_entries[num].flags |= gran & 0xF0;
//     gdt_entries[num].access = access;
// }