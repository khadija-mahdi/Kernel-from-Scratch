#include <lib/lib.h>
#include <drivers/vga/vga.h>

// #include <lib/lib.h>
// #include <kernel/vga.h>
// #include "../kernel/vga.h"
int ft_atoi(const char *str)
{
    long result = 0;
    int sign = 1;

    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;

    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }

    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return (int)(result * sign);
}

void memset(void *dest, char val, uint32_t count)
{
    char *temp = (char *)dest;
    for (; count != 0; count--)
    {
        *temp++ = val;
    }
}

// int strcmp(const char *s1, const char *s2)
// {
//     while (*s1 && (*s1 == *s2))
//     {
//         s1++;
//         s2++;
//     }
//     return *(unsigned char *)s1 - *(unsigned char *)s2;
// }
