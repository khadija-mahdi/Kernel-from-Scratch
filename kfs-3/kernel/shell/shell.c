#include <kernel/shell/shell.h>
#include <drivers/vga/vga.h>
#include <drivers/keyboard/keyboard.h>

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
    else if (strcmp(key_buffer[current_screen], "shutdown"))
    {
        shutdown();
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
    else
    {
        printk_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, "Unknown command: %s\n", key_buffer[current_screen]);
    }
}