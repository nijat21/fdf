#include "fdf.h"

char *read_file(int fd)
{
    char *map;

    map = get_next_line(fd);
    if (!map)
    {
        free(map); // leak
        return NULL;
    }
    return map;
}

void draw_line(t_vars *vars, int begX, int begY, int endX, int endY, int color)
{
    double deltaX = endX - begX;
    double deltaY = endY - begY;
    int pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));

    deltaX /= pixels;
    deltaY /= pixels;

    double pixX = begX;
    double pixY = begY;
    while (pixels)
    {
        mlx_pixel_put(vars->mlx, vars->win, pixX, pixY, color);
        pixX += deltaX;
        pixY += deltaY;
        pixels--;
    }
}

size_t ft_str_strlen(char **str)
{
    size_t count;

    count = 0;
    while (str[count])
        count++;
    return count;
}

void draw_map(t_vars *vars, char **map)
{
    int color = rgb_to_int(1, 1, 1);
    int scale = 15;
    size_t i = 0;
    size_t j;

    // Draw all horizontal lines (connect all x points in each row)
    while (i < ft_str_strlen(map))
    {
        j = 0;
        while (j + 1 < ft_strlen(map[i]))
        {
            draw_line(vars, j * scale, i * scale, (j + 1) * scale, i * scale, color);
            j++;
        }
        i++;
    }

    // Draw all vertical lines (connect all y points in each column)
    j = 0;
    while (j < ft_strlen(map[0])) // Assuming all rows have same length
    {
        i = 0;
        while (i + 1 < ft_str_strlen(map))
        {
            draw_line(vars, j * scale, i * scale, j * scale, (i + 1) * scale, color);
            i++;
        }
        j++;
    }
}

int main(int ac, char *av[])
{
    t_vars *vars;
    int fd;
    char *temp_map;
    char **map;
    int x = 1024;
    int y = 768;

    if (ac != 2)
        return 0;
    fd = open(av[1], O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return 0;
    }
    temp_map = read_file(fd);
    if (!temp_map)
    {
        ft_putstr_fd("Couldn't read the map\n", 2);
        close(fd);
        return 0;
    }
    map = ft_split(temp_map, '\n');
    if (!map)
    {
        ft_putstr_fd("Couldn't split the map\n", 2);
        return 0;
    }
    // ft_printf("ft_split map:\n");
    // for (int i = 0; map[i]; i++)
    //     ft_printf("%s\n", map[i]);

    vars = malloc(sizeof(t_vars));
    if (!vars)
        return 0;

    vars->mlx = mlx_init();
    vars->win = mlx_new_window(vars->mlx, x, y, "nismayil's fdf");

    // Clear the window before drawing
    mlx_clear_window(vars->mlx, vars->win);
    draw_map(vars, map);

    mlx_key_hook(vars->win, key_hook, vars);
    mlx_hook(vars->win, 17, 1L << 17, close_win, vars);
    mlx_loop(vars->mlx);

    free(vars);
    return 0;
}
