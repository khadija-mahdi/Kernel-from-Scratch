/* lib/types.h - Basic type definitions */
#include "keyboard.h"

#ifndef TYPES_H
#define TYPES_H

/* Unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/* Signed integer types */
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;


/* Size type */
typedef uint32_t size_t;

/*static */
 

/* NULL pointer */
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
void printk(const char *format, ...);
void printk_color(char fg, char bg, const char *format, ...);
#endif /* TYPES_H */