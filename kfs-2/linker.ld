SECTIONS
{
    . = 0x00000800;
    .gdt : {
        gdt_entries = .;
        *(.gdt)
    }
    
    . = 0x00100000;        /* Kernel starts at 1MB */
    
    .text : {
        *(.multiboot)
        *(.text)
    }
    
    .rodata : {
        *(.rodata)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        *(.bss)
    }
}