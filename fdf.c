#include "fdf.h"

int open_window(char *map)
{
    (void)map;
    return 0;
}

char *read_file(int fd)
{
    char *map;

    map = get_next_line(fd);
    if (!map)
    {
        ft_printf("map's empty");
        return NULL;
    }
    open_window(map);
    return map;
}

int main(int ac, char *av[])
{
    if (ac < 2)
        return 0;
    int fd;

    fd = open(av[1], O_RDONLY);
    if (fd == -1)
    {
        ft_printf("Can't access the file with this name\n");
        return 0;
    }
}
