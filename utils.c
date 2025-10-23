#include "fdf.h"

int rgb_to_int(double r, double g, double b)
{
    int color = 0;
    color |= (int)(b * 255);
    color |= (int)(g * 255) << 8;
    color |= (int)(r * 255) << 16;
    return (color);
}

int normalize_y(int curr, int total)
{
    return total - curr;
}

void free_char_arr(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
    arr = NULL;
}

void free_t_point_arr(t_point **arr)
{
    int i;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
    arr = NULL;
}

size_t ft_str_strlen(char **str)
{
    size_t count;

    count = 0;
    while (str[count])
        count++;
    return count;
}