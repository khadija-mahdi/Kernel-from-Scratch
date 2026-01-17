#include <kernel/shell/shell.h>
#include <drivers/vga/vga.h>
#include <lib/lib.h>

void reboot(){
    printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Rebooting...\n");
    // 0xFE to port 0x64
    port_byte_out(0x64, 0xFE);
}

// void shutdown(){
//     printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Shutting down...\n");

// }

void help(){
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, "Available commands:\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - reboot: Reboot the system\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - shutdown: Shutdown the system\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - help: Display this help message\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - halt: Halt the system\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " - clear: Clear the screen\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " -stack_trace: Print the current stack trace\n");
    printk_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, " -stack_dump: Dump the current stack contents\n");
}

void halt(){
    printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Halting the system...\n");
    asm volatile ("hlt");
}

void clear_screen(){
    terminal_clear(true);
    // printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "> ");
}


void print_stack_trace()
{
    uint32_t esp = get_esp();
    uint32_t ebp = get_ebp();  // ← Get EBP register, don't read from ESP
    
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "-----------------------------------------------\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Stack Trace:\n");
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Current ESP is at %x.\n", esp);
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Current EBP is at %x.\n", ebp);
    
    while (ebp != 0)
    {
        uint32_t return_address = *((uint32_t *)(ebp + 4));
        printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "Return address: %x (from EBP: %x)\n", 
                     return_address, ebp);
        
        ebp = *((uint32_t *)ebp);  // Follow the chain
        
        // Safety check to prevent infinite loops
        if (ebp < 0x100000 || ebp > 0x4000)
            break;
    }
    
    printk_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK, "-----------------------------------------------\n");
}


void stack_dump(){
    uint32_t esp = get_esp();
    printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "Stack Dump starting from ESP: %x\n", esp);
    for(int i = 0; i < 7; i++){
        uint32_t *addr = (uint32_t *)(esp + i * 4);
        uint32_t value = *addr;
        printk_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK, "Address: %x, Value: %x\n", addr, value);
    }
}
