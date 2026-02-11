#include <kernel/shell/shell.h>
#include <kernel/memory/panic.h>
#include <drivers/vga/vga.h>
#include <drivers/keyboard/keyboard.h>
#include <lib/lib.h>

bool strcmp(const char *str1, const char *str2)
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
            return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

bool is_whitespace(char *c)
{
    int i = 0;
    while (c[i] != '\0')
    {
        if (c[i] != ' ')
            return false;
        i++;
    }
    return true;
}

void micro_shell()
{
    // char user_input[256] = key_buffer[current_screen];
    if (strcmp(key_buffer[current_screen], "") || is_whitespace(key_buffer[current_screen]))
    {
        return;
    }
    else if (strcmp(key_buffer[current_screen], "reboot"))
    {
        reboot();
    }
    else if (strcmp(key_buffer[current_screen], "help"))
    {
        help();
    }
    else if (strcmp(key_buffer[current_screen], "halt"))
    {
        halt();
    }
    else if (strcmp(key_buffer[current_screen], "clear"))
    {
        clear_screen();
    }
    else if (strcmp(key_buffer[current_screen], "stack_trace"))
    {
        print_stack_trace();
    }
    else if (strcmp(key_buffer[current_screen], "stack_dump"))
    {
        stack_dump();
    }
    else if (strcmp(key_buffer[current_screen], "test_panic"))
    {
        test_panic();
    }
    else if (strcmp(key_buffer[current_screen], "test_fatal"))
    {
        kpanic(PANIC_FATAL, "Testing fatal panic");
    }
    else if (strcmp(key_buffer[current_screen], "test_oom"))
    {
        kpanic(PANIC_OOM, "Testing out of memory panic");
    }
    else if (strcmp(key_buffer[current_screen], "test_stack"))
    {
        kpanic(PANIC_STACK, "Testing stack overflow panic");
    }
    else if (strcmp(key_buffer[current_screen], "test_init"))
    {
        kpanic(PANIC_INIT, "Testing initialization failure");
    }
    else if (strcmp(key_buffer[current_screen], "test_warning"))
    {
        kpanic(PANIC_WARNING, "Testing warning (should continue!)");
    }
    else if (strcmp(key_buffer[current_screen], "test_pagefault"))
    {
        kpanic(PANIC_PAGE_FAULT, "Testing page fault at 0xDEADBEEF");
    }
    else if (strcmp(key_buffer[current_screen], "test_dfree"))
    {
        kpanic(PANIC_DOUBLE_FREE, "Testing double free at 0xC0001234");
    }
    else if (strcmp(key_buffer[current_screen], "test_corrupt"))
    {
        kpanic(PANIC_HEAP_CORRUPT, "Testing heap corruption detection");
    }
    else if (strcmp(key_buffer[current_screen], "test_divzero"))
    {
        kpanic(PANIC_DIVISION_ZERO, "Testing division by zero");
    }
    else if (strcmp(key_buffer[current_screen], "test_gpf"))
    {
        kpanic(PANIC_GPF, "Testing general protection fault");
    }
    else
    {
        printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Unknown command: %s\n", key_buffer[current_screen]);
    }
}