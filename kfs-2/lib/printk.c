#include <stdarg.h>
#include "../kernel/vga.h"

static void printk_puts_color(const char *str, char color)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            screen_cursor_col[current_screen] = 0;
            if (++screen_cursor_row[current_screen] == VGA_HEIGHT)
                scroll();
        }
        else
        {
            terminal_putentryat(str[i], color, screen_cursor_col[current_screen], screen_cursor_row[current_screen]);
            if (++screen_cursor_col[current_screen] == VGA_WIDTH)
            {
                screen_cursor_col[current_screen] = 0;
                if (++screen_cursor_row[current_screen] == VGA_HEIGHT)
                    scroll();
            }
        }
    }
    set_cursor(get_offset(screen_cursor_col[current_screen], screen_cursor_row[current_screen]));
}

void printk(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    void print_hex(unsigned int num)
    {
        char buf[9];
        buf[8] = '\0';
        for (int i = 7; i >= 0; --i) {
            unsigned int nibble = num & 0xF;
            if (nibble < 10) buf[i] = '0' + nibble;
            else buf[i] = 'a' + (nibble - 10);
            num >>= 4;
        }
        int start = 0;
        while (start < 7 && buf[start] == '0') start++;
        terminal_writestring(&buf[start]);
    }

    for (size_t i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%') {
            i++;
            switch (format[i])
            {
                case 'd': print_int(va_arg(args,int)); break;
                case 'x': print_hex(va_arg(args,unsigned int)); break; 
                case 's': terminal_writestring(va_arg(args,char*)); break;
                case 'c': terminal_putchar((char)va_arg(args,int)); break;
                case '%': terminal_putchar('%'); break;
                default: terminal_putchar(format[i]); break;
            }
        } else {
            terminal_putchar(format[i]);
        }
    }

    va_end(args);
}

void printk_color(char fg, char bg, const char *format, ...)
{
    char color = vga_entry_color(fg, bg);
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%') {
            i++;
            switch (format[i])
            {
                case 'd': {
                    int num = va_arg(args,int);
                    char buf[digit_count(num)+1];
                    itoa(num, buf);
                    printk_puts_color(buf, color);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args,unsigned int);
                    char buf[9]; // 8 digits + null
                    itoa(num, buf);
                    printk_puts_color(buf, color);
                    break;
                }
                case 's': {
                    char *str = va_arg(args,char*);
                    printk_puts_color(str, color);
                    break;
                }
                case 'c': {
                    char ch = (char)va_arg(args,int);
                    char tmp[2] = {ch, 0};
                    printk_puts_color(tmp, color);
                    break;
                }
                case '%': {
                    char tmp[2] = {'%', 0};
                    printk_puts_color(tmp, color);
                    break;
                }
                default: {
                    char tmp[2] = {format[i], 0};
                    printk_puts_color(tmp, color);
                    break;
                }
            }
        } else {
            char tmp[2] = {format[i], 0};
            printk_puts_color(tmp, color);
        }
    }

    va_end(args);
}
