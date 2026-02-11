/* lib/lib.h - Basic type definitions */
// #include "keyboard.h"

#ifndef LIB_H
#define LIB_H

// #include <lib/keyboard.h>

#include <drivers/keyboard/keyboard.h>

/* Unsigned integer LIB */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/* Signed integer LIB */
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;



/* Size type */
typedef uint32_t size_t;

/*static */


/* NULL pointeappenr */
#define CEIL_DIV(a,b) (((a + b) - 1)/b)
#define NULL ((void *)0)

/* Boolean type */
typedef enum
{
    false = 0,
    true = 1
} bool;


#define LLONG_MAX 18446744073709551615ULL;

void itoa(int num, char *number);
unsigned int digit_count(int num);
unsigned int strlen(const char *str);
void printk_color(char fg, char bg, const char *format, ...);
uint32_t get_esp();
uint32_t get_ebp();
void memset(void *dest, char val, uint32_t count);
// int strcmp(const char *s1, const char *s2);


#endif /* LIB_H */