
#ifndef VGA_H
#define VGA_H

#include "../lib/types.h"

/* VGA color codes */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_WHITE = 15,
};

/* VGA text mode dimensions */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_writestring(const char* str);
void terminal_clear(void);


#endif /* VGA_H */