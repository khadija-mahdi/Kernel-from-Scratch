#include <kernel/memory/panic.h>
#include <drivers/vga/vga.h>
#include <kernel/shell/shell.h>

void panicFatal()
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** KERNEL PANIC ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  A critical error has occurred. The system will halt.\n");
    print_stack_trace();
    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicOutOfMemory(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** OUT OF MEMORY ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n\n", message);

    print_stack_trace();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicStackOverflow(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** STACK OVERFLOW ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n\n", message);

    uint32_t esp, ebp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "  ESP: 0x%x\n", esp);
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "  EBP: 0x%x\n", ebp);
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "  Stack bottom: 0xC010A000\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "  Stack top:    0xC012A000\n\n");

    if (esp < 0xC0109000)
    {
        printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK,
                     "  ERROR: ESP below stack bottom!\n");
    }

    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicInit(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** INITIALIZATION FAILED ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n", message);
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  System cannot continue.\n");
    halt();
}

void panicWarning(const char *message)
{
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "\n*** WARNING ***\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "  %s\n", message);
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
                 "  System will attempt to continue.\n\n");
}

void panicPageFault(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** PAGE FAULT ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n", message);
    print_stack_trace();
    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicDoubleFree(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** DOUBLE FREE DETECTED ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n", message);
    print_stack_trace();
    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicHeapCorrupt(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** HEAP CORRUPTION DETECTED ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n", message);
    print_stack_trace();
    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicDivisionZero(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** DIVISION BY ZERO ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n", message);
    print_stack_trace();
    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void panicGPF(const char *message)
{
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n*** GENERAL PROTECTION FAULT ***\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "  %s\n", message);
    print_stack_trace();
    stack_dump();
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
                 "\n  System halted.\n");
    halt();
}

void kpanic(panic_type_t type, const char *message)
{
    switch (type)
    {
    case PANIC_FATAL:
        panicFatal();
        break;

    case PANIC_OOM:
        panicOutOfMemory(message);
        break;

    case PANIC_STACK:
        panicStackOverflow(message);
        break;

    case PANIC_INIT:
        panicInit(message);
        break;

    case PANIC_WARNING:
        panicWarning(message);
        break;

    case PANIC_PAGE_FAULT:
        panicPageFault(message);
        break;
    case PANIC_DOUBLE_FREE:
        panicDoubleFree(message);
        break;
    case PANIC_HEAP_CORRUPT:
        panicHeapCorrupt(message);
        break;
    case PANIC_DIVISION_ZERO:
        panicDivisionZero(message);
        break;
    case PANIC_GPF:
        panicGPF(message);
        break;
    }
}