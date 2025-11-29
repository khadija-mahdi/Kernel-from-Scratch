SECTIONS
{
    . = 1M;

    .text :
    {
        *(.multiboot_header)
        *(.text)
    }
    .data :
    {
        *(.data)
    }

    /* Read-write uninitialized data */
    .bss :
    {
        *(.bss)
    }
}