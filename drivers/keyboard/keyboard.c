// #include <kernel/vga.h>
// #include <lib/types.h>

#include <drivers/vga/vga.h>
#include <lib/types.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)
char key_buffer[256];

void append(char s[], char n)
{
    int len = strlen(s);
    s[len] = n;
    s[len + 1] = '\0';
}

bool backspace()
{
    int len = strlen(key_buffer);
    if (len > 0)
    {
        key_buffer[len - 1] = '\0';
        return true;
    }
    else
    {
        return false;
    }
}

int8_t inb(uint16_t port)
{
    int8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

void outb(uint16_t port, int8_t data)
{
    asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

char get_input_keycode()
{
    if (inb(0x64) & 0x01)
        return inb(KEYBOARD_PORT);
    return 0;
}

void wait_for_io(uint32_t timer_count)
{
    while (1)
    {
        __asm__ volatile("nop");
        timer_count--;
        if (timer_count <= 0)
            break;
    }
}

void sleep(uint32_t timer_count)
{
    wait_for_io(timer_count);
}

void keyboard_input()
{
    char ch = 0;
    char keycode = 0;

    while (1)
    {
        keycode = get_input_keycode();
        if (keycode == 0 || keycode & 0x80)
        {
            handle_key_release(keycode & 0x7F);
            continue;
        }
        if (keycode == KEY_ENTER)
        {
            terminal_putchar('\n');
            printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "key_buffer: %s\n", key_buffer);
            printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "> ");
            key_buffer[0] = '\0';
        }
        else if (keycode == KEY_BACKSPACE)
        {
            if (backspace(key_buffer))
                remove_last_char();
        }
        else
        {
            ch = get_ascii_char(keycode);
            append(key_buffer, ch);
            if (ch == 0x03)
            {
                terminal_writestring("^C\n");
                continue;
            }
            else if (ch == 0x10)
            {
                restoreScreen();
                if (screen_cursor_row[current_screen] == 0)
                {
                    printk_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK, "> ");
                }
                key_buffer[0] = '\0';
            }
            else if (ch == 0x09)
                terminal_writestring("    ");
            else if (ch != 0)
                terminal_putchar(ch);
        }
        sleep(0x00FFFFFF);
    }
}