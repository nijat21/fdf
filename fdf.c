#include "fdf.h"

// char *read_file(int fd)
// {
//     char *map;

//     map = get_next_line(fd);
//     if (!map)
//     {
//         ft_printf("map's empty");
//         return NULL;
//     }
//     return map;
// }

void draw_line(void *mlx, void *win, int begX, int begY, int endX, int endY, int color)
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
        mlx_pixel_put(mlx, win, pixX, pixY, color);
        pixX += deltaX;
        pixY += deltaY;
        pixels--;
    }
}

int main(int ac, char *av[])
{
    (void)av;
    (void)ac;
    // if (ac < 2)
    //     return 0;
    // int fd;

    // fd = open(av[1], O_RDONLY);
    // if (fd == -1)
    // {
    //     ft_printf("Can't access the file with this name\n");
    //     return 0;
    // }

    int x = 1024;
    int y = 768;
    void *mlx = mlx_init();
    void *win = mlx_new_window(mlx, x, y, "nismayil's fdf");

    mlx_clear_window(mlx, win); // Clears the window (sets to black by default)

    // Draw horizontal line (cross)
    draw_line(mlx, win, 0, y, x, 0, rgb_to_int(1, 1, 1));
    draw_line(mlx, win, 0, 0, x, y, rgb_to_int(1, 1, 1));

    mlx_loop(mlx);
    return 0;
}
