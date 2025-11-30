SECTIONS
{
    . = 0x00100000;

    .text :
    {
        *(.multiboot_header)
        *(.text)
    }
    .data :
    {
        *(.data)
    }

}