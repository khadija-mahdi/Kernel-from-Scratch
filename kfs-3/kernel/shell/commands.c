#include <kernel/shell/shell.h>
#include <kernel/memory/kmalloc.h>
#include <kernel/memory/panic.h>
#include <drivers/vga/vga.h>
#include <lib/lib.h>

void reboot()
{
    printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Rebooting...\n");
    // 0xFE to port 0x64
    port_byte_out(0x64, 0xFE);
}

void help()
{
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, "Available commands:\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - reboot: Reboot the system\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - help: Display this help message\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - halt: Halt the system\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - clear: Clear the screen\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - stack_trace: Print the current stack trace\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - stack_dump: Dump the current stack contents\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - test_panic: Test panic handlers (see options)\n");
}

void test_panic()
{
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "\n=== Panic Test Menu ===\n");
    printk_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK, "Available panic tests:\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  1. test_fatal      - Fatal panic\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  2. test_oom        - Out of memory\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  3. test_stack      - Stack overflow\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  4. test_init       - Init failure\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  5. test_warning    - Warning (non-fatal)\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  6. test_pagefault  - Page fault\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  7. test_dfree      - Double free\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  8. test_corrupt    - Heap corruption\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  9. test_divzero    - Division by zero\n");
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "  10. test_gpf       - General protection fault\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "\nType command to test (e.g., 'test_fatal')\n\n");
}

void halt()
{
    printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Halting the system...\n");
    asm volatile("hlt");
}

void clear_screen()
{
    terminal_clear(true);
    // printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "> ");
}

void print_stack_trace()
{
    uint32_t esp = get_esp();
    uint32_t ebp = get_ebp(); // ← Get EBP register, don't read from ESP

    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "-----------------------------------------------\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Stack Trace:\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Current ESP is at %x.\n", esp);
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Current EBP is at %x.\n", ebp);
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Stack range: %x - %x\n", 0xC010A000, 0xC012A000);

    // Account for function call overhead - ESP might be slightly outside due to call chain
    if (esp >= 0xC0109000 && esp <= 0xC012B000)
    {
        printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Stack is valid!\n\n");
        
        // Walk the stack frame chain
        printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Call stack:\n");
        
        // If EBP is 0, it means we're at the bottom of the stack (initial state from boot.s)
        if (ebp == 0) {
            printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, 
                        "  [Bottom of stack] EBP=0 (set by boot.s before kernel_main)\n");
        } else if (ebp < 0xC0109000 || ebp >= 0xC012B000) {
            printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, 
                        "  [Warning] EBP outside stack range: %x\n", ebp);
        } else {
            // Walk frames
            int frame_count = 0;
            while (ebp != 0 && frame_count < 10)
            {
                // Check if EBP is within expanded valid stack range
                if (ebp < 0xC0109000 || ebp >= 0xC012B000) {
                    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, 
                                "  [Frame %d] EBP out of range: %x (stopping)\n", frame_count, ebp);
                    break;
                }
                
                uint32_t return_address = *((uint32_t *)(ebp + 4));
                printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, 
                            "  [%d] Return addr: %x (EBP: %x)\n",
                            frame_count, return_address, ebp);

                uint32_t prev_ebp = *((uint32_t *)ebp);
                
                // Stop if chain ends or loops back
                if (prev_ebp == 0 || prev_ebp <= ebp)
                    break;
                    
                ebp = prev_ebp;
                frame_count++;
            }
            
            if (frame_count == 0) {
                printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, 
                            "  [No frames] Stack not deep enough or no frame pointers\n");
            }
        }
    }
    else
    {
        printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Stack overflow or corruption detected!\n");
    }

    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "-----------------------------------------------\n");
}

void stack_dump()
{
    uint32_t esp = get_esp();
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "Stack Dump starting from ESP: %x\n", esp);
    
    // Account for function call overhead - allow small margin
    if (esp < 0xC0109000 || esp > 0xC012B000) {
        printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "ESP outside valid stack range!\n");
        printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "Stack range: %x - %x\n", 0xC010A000, 0xC012A000);
        return;
    }
    
    for (int i = 0; i < 16; i++)
    {
        uint32_t *addr = (uint32_t *)(esp + i * 4);
        
        // Stop if we go beyond stack top
        if ((uint32_t)addr >= 0xC012A000)
            break;
            
        uint32_t value = *addr;
        printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "[ESP+%02d] %x: %x\n", i*4, addr, value);
    }
}

void panicTest(char *type)
{
    if (strcmp(type, "stack_overflow"))
    {
        panicStackOverflow("This is a stack overflow test.");
    }
    else if (strcmp(type, "double_free"))
    {
        panicDoubleFree("This is a double free test.");
    }
    else if (strcmp(type, "heap_corrupt"))
    {
        panicHeapCorrupt("This is a heap corruption test.");
    }
    else if (strcmp(type, "division_zero"))
    {
        panicDivisionZero("This is a division by zero test.");
    }
    else if (strcmp(type, "gpf"))
    {
        panicGPF("This is a general protection fault test.");
    }
    else
    {
        printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Unknown panic type: %s\n", type);
    }
}
