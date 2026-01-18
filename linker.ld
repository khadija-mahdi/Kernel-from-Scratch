SECTIONS
{
    . = 0x00000800;
    .gdt : {
        gdt_entries = .;
        *(.gdt)
    }
    
    . = 0x00100000;        /* Kernel starts at 1MB */
    
    .text ALIGN(4K) : {
        *(.multiboot_header)
        *(.multiboot)
        *(.text)
    }
    
    .rodata ALIGN(4K) : {
        *(.rodata)
    }
    
    .data ALIGN(4K) : {
        *(.data)
    }
    
    .bss ALIGN(4K) : {
        *(.bss)
        *(COMMON)
    }
}