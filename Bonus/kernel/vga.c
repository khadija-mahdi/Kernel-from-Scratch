#include "vga.h"

/* Definitions for globals declared in vga.h */
short *const VGA_MEMORY = (short *)0xB8000;
char screen[MAX_SCREENS][VGA_WIDTH * VGA_HEIGHT * 2];
int current_screen = 0;

char terminal_color;
size_t screen_cursor_row[MAX_SCREENS];
size_t screen_cursor_col[MAX_SCREENS];

unsigned int strlen(const char *str)
{
    unsigned int length = 0;
    while (str[length])
        length++;
    return length;
}

unsigned int digit_count(int num)
{
    unsigned int count = 0;
    if (num == 0)
        return 1;
    while (num > 0)
    {
        count++;
        num = num / 10;
    }
    return count;
}

void itoa(int num, char *number)
{
    unsigned int dgcount = digit_count(num);
    int index = dgcount - 1;
    char x;
    if (num == 0 && dgcount == 1)
    {
        number[0] = '0';
        number[1] = '\0';
    }
    else
    {
        while (num != 0)
        {
            x = num % 10;
            number[index] = x + '0';
            index--;
            num = num / 10;
        }
        number[dgcount] = '\0';
    }
}

static inline short vga_entry(unsigned char c, char color)
{
    return (short)c | (short)color << 8;
}

char vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

void memory_copy()
{
    for (int y = 1; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }
}

void saveCurrentScreen()
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i++)
    {
        screen[current_screen][i] = ((char *)VGA_MEMORY)[i];
    }
}

void scroll()
{
    memory_copy();

    for (int x = 0; x < VGA_WIDTH; x++)
    {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
    }

    screen_cursor_row[current_screen] = VGA_HEIGHT - 1;
    screen_cursor_col[current_screen] = 0;

    saveCurrentScreen();
}

void init_screens()
{
    for (int i = 0; i < MAX_SCREENS; i++)
    {
        screen_cursor_row[i] = 0;
        screen_cursor_col[i] = 0;

        for (int j = 0; j < VGA_WIDTH * VGA_HEIGHT * 2; j += 2)
        {
            screen[i][j] = ' ';
            screen[i][j + 1] = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        }
    }
}

void terminal_initialize()
{
    init_screens();
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
}

int get_offset(int col, int row)
{
    return 2 * (row * VGA_WIDTH + col);
}

void restoreScreen()
{
    int next_screen = (current_screen + 1) % MAX_SCREENS;
    if (next_screen < 0 || next_screen >= MAX_SCREENS || next_screen == current_screen)
        return;

    saveCurrentScreen();
    current_screen = next_screen;

    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i++)
    {
        ((char *)VGA_MEMORY)[i] = screen[current_screen][i];
    }

    set_cursor(get_offset(screen_cursor_col[current_screen], screen_cursor_row[current_screen]));
}

void terminal_putentryat(char c, char color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    VGA_MEMORY[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        screen_cursor_col[current_screen] = 0;
        if (++screen_cursor_row[current_screen] == VGA_HEIGHT)
            scroll();
        set_cursor(get_offset(screen_cursor_col[current_screen], screen_cursor_row[current_screen]));
        return;
    }

    terminal_putentryat(c, terminal_color, screen_cursor_col[current_screen], screen_cursor_row[current_screen]);

    if (++screen_cursor_col[current_screen] == VGA_WIDTH)
    {
        screen_cursor_col[current_screen] = 0;
        if (++screen_cursor_row[current_screen] == VGA_HEIGHT)
            scroll();
    }
    set_cursor(get_offset(screen_cursor_col[current_screen], screen_cursor_row[current_screen]));
}

void terminal_writestring(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        terminal_putchar(str[i]);
    }
}

void terminal_clear(void)
{
    terminal_initialize();
}

void print_int(int num)
{
    char str_num[digit_count(num) + 1];
    itoa(num, str_num);
    terminal_writestring(str_num);
}

void remove_last_char()
{
    if (screen_cursor_col[current_screen] == 0)
        return;
    screen_cursor_col[current_screen]--;

    terminal_putentryat(' ', terminal_color, screen_cursor_col[current_screen], screen_cursor_row[current_screen]);
    set_cursor(get_offset(screen_cursor_col[current_screen], screen_cursor_row[current_screen]));
}
