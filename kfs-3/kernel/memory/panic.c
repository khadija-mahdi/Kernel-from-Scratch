#include <kernel/memory/panic.h>
#include <drivers/vga/vga.h>

void kernel_panic(const char *message)
{
    /* Disable interrupts immediately */
    asm volatile("cli");

    /* Grab register snapshot */
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, ebp, esp;
    uint32_t cr0, cr2, cr3;

    asm volatile("mov %%eax, %0" : "=m"(eax));
    asm volatile("mov %%ebx, %0" : "=m"(ebx));
    asm volatile("mov %%ecx, %0" : "=m"(ecx));
    asm volatile("mov %%edx, %0" : "=m"(edx));
    asm volatile("mov %%esi, %0" : "=m"(esi));
    asm volatile("mov %%edi, %0" : "=m"(edi));
    asm volatile("mov %%ebp, %0" : "=m"(ebp));
    asm volatile("mov %%esp, %0" : "=m"(esp));
    asm volatile("mov %%cr0, %%eax; mov %%eax, %0" : "=m"(cr0) :: "eax");
    asm volatile("mov %%cr2, %%eax; mov %%eax, %0" : "=m"(cr2) :: "eax");
    asm volatile("mov %%cr3, %%eax; mov %%eax, %0" : "=m"(cr3) :: "eax");

    /* Red banner */
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
        "\n\n"
        "========================================\n"
        "          *** KERNEL PANIC ***          \n"
        "========================================\n");

    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED, "  %s\n\n", message);

    /* Register dump */
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
        "  Register dump:\n"
        "    EAX=%x  EBX=%x  ECX=%x  EDX=%x\n"
        "    ESI=%x  EDI=%x  EBP=%x  ESP=%x\n"
        "    CR0=%x  CR2=%x  CR3=%x\n",
        eax, ebx, ecx, edx,
        esi, edi, ebp, esp,
        cr0, cr2, cr3);

    /* Stack trace */
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "\n  Stack trace:\n");
    uint32_t *frame = (uint32_t *)ebp;
    for (int depth = 0; depth < 10 && frame; depth++) {
        uint32_t ret_addr = frame[1];
        if (ret_addr == 0)
            break;
        printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK,
            "    #%d  return addr: %x  (frame: %x)\n", depth, ret_addr, (uint32_t)frame);
        frame = (uint32_t *)frame[0];
        if ((uint32_t)frame < 0x1000)
            break;
    }

    printk_color(VGA_COLOR_WHITE, VGA_COLOR_RED,
        "\n  System halted. Please reboot.\n"
        "========================================\n");

    /* Halt forever */
    for (;;)
        asm volatile("hlt");
}
