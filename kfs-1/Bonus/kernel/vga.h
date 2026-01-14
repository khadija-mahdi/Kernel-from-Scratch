
#ifndef VGA_H
#define VGA_H

#include "../lib/types.h"

/* VGA color codes */
enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
};

/* VGA text mode dimensions */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/*VGA */
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define MAX_SCREENS 3

extern short *const VGA_MEMORY;
extern char screen[MAX_SCREENS][VGA_WIDTH * VGA_HEIGHT * 2];
extern int current_screen;

extern char terminal_color;
extern size_t screen_cursor_row[MAX_SCREENS];
extern size_t screen_cursor_col[MAX_SCREENS];

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_writestring(const char *str);
void terminal_clear(void);
void print_int(int num);
void remove_last_char();
void handle_key_release(char keycode);
char get_ascii_char(char keycode);
void restoreScreen();
void terminal_putentryat(char c, char color, size_t x, size_t y);
void scroll(void);
void set_cursor(int offset);

/* helpers exposed for other modules */
int get_offset(int col, int row);
char vga_entry_color(enum vga_color fg, enum vga_color bg);
#endif /* VGA_H */