#include "fdf.h"

char *read_file(int fd)
{
    char *map;

    map = get_next_line(fd);
    if (!map)
        return NULL;
    return map;
}

/* To do
    1. Adjusted spacing for the image size V
    2. Connect dots with lines -> pixel-adjusted draw_line function
    3. Calculate the starting point so that the image is centered in the screen
 */
void draw_line(int x1, int y1, int x2, int y2, char *data, int bpp, int size_line, int color)
{
    int x;
    int y;
    int dx;
    int dy;
    int p;

    x = x1;
    y = y1;
    dx = x2 - x1;
    dy = y2 - y1;
    p = 2 * dy - dx;

    while (x <= x2)
    {
        *(int *)(data + (y * size_line + x * (bpp / 8))) = color;
        x++;
        if (p < 0)
            p += 2 * dy;
        else if (p > 0)
        {
            p += 2 * dy - 2 * dx;
            y++;
        }
    }
}

// Slope-independent draw_line
/*
void draw_line(int x1, int y1, int x2, int y2, char *data, int bpp, int size_line, int color)
{
    int dx;
    int dy;
    int err;
    int sx;
    int sy;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    err = dy - dx;

    if (x1 < x2)
        sx = 1;
    else
        sx = -1;

    if (y1 < y2)
        sy = 1;
    else
        sy = -1;

    while (1)
    {
        printf("Drawing frame\n");
        *(int *)(data + (y1 * size_line + x1 * (bpp / 8))) = color;
        if (x1 == x2 && y1 == y2)
            break;

        if (2 * err < -dy)
        {
            err -= dy;
            x1 += sx;
        }
        else if (2 * err < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}
     */

void draw_put_image(t_win *window, t_image *img, t_map *map)
{
    char *data;
    int bpp;
    int size_line;
    int endian;

    int w_space = img->width / largest_row(map);
    int h_space = img->height / map->nrows;
    int row;
    int col;

    data = mlx_get_data_addr(img->img_ptr, &bpp, &size_line, &endian);
    row = 0;
    while (row < map->nrows)
    {
        col = 0;
        while (col < map->rows[row].ncols - 1)
        {
            draw_line(
                map->rows[row].cols[col].x * w_space,
                map->rows[row].cols[col].y * h_space,
                map->rows[row].cols[col + 1].x * w_space,
                map->rows[row].cols[col].y * h_space,
                data, bpp, size_line, map->rows[row].cols[col].color);
            col++;
        }
        row++;
    }

    mlx_put_image_to_window(window->mlx, window->win, img->img_ptr, 0, 0);
}
// draw_line(0, img->height / 2, img->width, img->height / 2,
//           data, bpp, size_line, curr.color);

t_map *parse_store_map(int fd)
{
    char *temp_map;
    char **str_map;
    char **map_row;
    char **parts;
    size_t row;
    size_t nrows;
    size_t col;
    size_t ncols;
    t_map *map;

    temp_map = read_file(fd);
    close(fd);
    if (!temp_map)
    {
        ft_putstr_fd("Couldn't read the map\n", 2);
        return NULL;
    }
    str_map = ft_split(temp_map, '\n');
    free(temp_map);
    if (!str_map)
    {
        ft_putstr_fd("Couldn't split the map\n", 2);
        return NULL;
    }
    nrows = ft_str_strlen(str_map);
    map = malloc(sizeof(t_map));
    if (!map)
    {
        ft_putstr_fd("Couldn't allocate memory for map\n", 2);
        free_char_arr(str_map);
        return NULL;
    }
    map->nrows = nrows;
    map->rows = malloc(sizeof(t_row) * nrows);
    if (!map->rows)
    {
        ft_putstr_fd("Couldn't allocate memory for map->rows\n", 2);
        free_char_arr(str_map);
        free_map(map);
        return NULL;
    }
    row = 0;
    while (row < nrows)
    {
        // ft_printf("row %d\n", row);
        map_row = ft_split(str_map[row], ' ');
        if (!map_row)
        {
            ft_putstr_fd("Couldn't split the map row\n", 2);
            free_char_arr(str_map);
            free_map(map);
            return NULL;
        }
        ncols = ft_str_strlen(map_row);
        map->rows[row].cols = malloc(sizeof(t_point) * ncols);
        if (!map->rows[row].cols)
        {
            ft_putstr_fd("Couldn't allocate memory for the row\n", 2);
            free_char_arr(str_map);
            free_map(map);
            return NULL;
        }
        col = 0;
        while (col < ncols)
        {
            parts = ft_split(map_row[col], ',');
            map->rows[row].cols[col].x = col;
            map->rows[row].cols[col].y = row;
            map->rows[row].cols[col].z = ft_atoi(parts[0]);
            if (parts[1])
                map->rows[row].cols[col].color = ft_atoi_base(parts[1], 16);
            else
                map->rows[row].cols[col].color = 0xFFFFFF;
            free_char_arr(parts);
            col++;
        }
        free_char_arr(map_row);
        map->rows[row].ncols = ncols;
        row++;
    }

    // ft_printf("Map has %d rows\n", map->nrows);
    // for (int i = 0; i < map->nrows; i++)
    // {
    //     t_row temp_row = map->rows[i];
    //     for (int j = 0; j < temp_row.ncols; j++)
    //         ft_printf("X: %d, Y: %d, Z: %d, C: %d\n", temp_row.cols[j].x, temp_row.cols[j].y, temp_row.cols[j].z, temp_row.cols[j].color);
    //     ft_printf("This row has %d columns\n", temp_row.ncols);
    // }

    return map;
}

int main(int ac, char *av[])
{
    t_win *window;
    t_image *img;
    t_map *map;
    int fd;

    if (ac != 2)
        return 0;
    fd = open(av[1], O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return 0;
    }

    window = malloc(sizeof(t_win));
    if (!window)
        return 0;

    window->mlx = mlx_init();
    window->width = 1920;
    window->height = 1400;
    window->win = mlx_new_window(window->mlx, window->width, window->height, "nismayil's fdf");

    // Clear the window before drawing
    mlx_clear_window(window->mlx, window->win);

    // Create an image
    img = malloc(sizeof(t_image));
    if (!img)
    {
        free(window);
        return 0;
    }
    img->width = 960;
    img->height = 700;
    img->img_ptr = mlx_new_image(window->mlx, img->width, img->height);
    if (!img->img_ptr)
    {
        ft_putstr_fd("Couldn't create image\n", 2);
        free(window);
        return 0;
    }

    map = parse_store_map(fd);
    draw_put_image(window, img, map);

    mlx_key_hook(window->win, key_hook, window);
    mlx_hook(window->win, 17, 1L << 17, close_win, window);
    mlx_loop(window->mlx);

    free(window);
    return 0;
}
