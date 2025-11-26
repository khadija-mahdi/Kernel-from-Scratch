ENTRY(_start)

SECTIONS
{
    . = 1M;          /* load at 1MB */
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss) }
}