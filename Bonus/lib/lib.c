#include "types.h"

int ft_atoi(const char *str)
{
    long result = 0;
    int sign = 1;

    // Skip spaces (ASCII 9–13 and 32)
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;

    // Handle sign
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }

    // Convert digits
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return (int)(result * sign);
}


