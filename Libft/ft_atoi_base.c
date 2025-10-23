#include "libft.h"

static int char_to_val(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    return (-1);
}

int ft_atoi_base(const char *str, int base)
{
    int i;
    int sign;
    long result;
    int val;

    if (base < 2 || base > 16)
        return (0);

    i = 0;
    sign = 1;
    result = 0;

    while (is_space(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    // optional prefix like "0x"
    if (base == 16 && str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
        i += 2;

    while ((val = char_to_val(str[i])) >= 0 && val < base)
    {
        result = result * base + val;
        i++;
    }
    result *= sign;
    if (!(result <= 2147483647 && result >= -2147483648))
        return (0);
    return ((int)result);
}
