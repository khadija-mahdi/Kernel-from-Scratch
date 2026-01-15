#include "stdint.h"

#define GDT_ADDR 0x00000800

struct  gdt_entry_struct
{
    uint16_t limit;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  flags;
    uint8_t  base_high;
}__attribute__((packed));

struct gdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

void init_gdt();
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
