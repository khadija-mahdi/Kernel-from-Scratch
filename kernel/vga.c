/* kernel/vga.c - VGA text mode driver implementation */

#include "vga.h"

/* VGA buffer is at physical address 0xB8000 */
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

/* Current cursor position */
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;

/* Create a VGA entry (character + color attribute) */
static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

/* Create color attribute byte */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

/* Initialize the terminal */
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    /* Clear entire screen */
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
}

/* Put character at specific position */
static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_MEMORY[index] = vga_entry(c, color);
}

/* Put character at current cursor position */
void terminal_putchar(char c) {
    /* Handle newline */
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
        return;
    }
    
    /* Put character at cursor */
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    
    /* Advance cursor */
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

/* Write a string to terminal */
void terminal_writestring(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        terminal_putchar(str[i]);
}

/* Clear the screen */
void terminal_clear(void) {
    terminal_initialize();
}