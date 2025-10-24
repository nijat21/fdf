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

// void draw_put_image(t_win *window, t_image *img, t_point **points)
// {
//     char *data;
//     int bpp;
//     int size_line;
//     int endian;

//     int spacing = 50;
//     size_t rows;
//     size_t cols;

//     data = mlx_get_data_addr(img->img_ptr, &bpp, &size_line, &endian);

//     while ()
//         for (int y = 0; y < img->height; y += spacing)
//             for (int x = 0; x < img->width; x += spacing)
//                 if (x - spacing >= 0 && y - spacing >= 0 &&)
//                     *(int *)(data + (y * size_line + x * (bpp / 8))) = color;
//     mlx_put_image_to_window(window->mlx, window->win, img->img_ptr, 0, 0);
// }

t_map *parse_store_map(int fd)
{
    char *temp_map;
    char **str_map;
    size_t row;
    size_t rows;
    size_t col;
    size_t cols;
    t_point **points;
    t_map *map;

    temp_map = read_file(fd);
    if (!temp_map)
    {
        ft_putstr_fd("Couldn't read the map\n", 2);
        close(fd);
        return NULL;
    }
    str_map = ft_split(temp_map, '\n');
    if (!str_map)
    {
        ft_putstr_fd("Couldn't split the map\n", 2);
        return NULL;
    }
    ft_printf("ft_split map:\n");
    rows = ft_str_strlen(str_map);
    points = malloc(sizeof(t_point) * rows);
    map = malloc(sizeof(t_map));
    map->nrows = rows;
    map->rows = malloc(sizeof(t_row) * rows);
    row = 0;
    while (row < rows)
    {
        // ft_printf("row %d\n", row);
        char **map_row = ft_split(str_map[row], ' ');
        if (!map_row)
        {
            ft_putstr_fd("Couldn't split the map row\n", 2);
            free_char_arr(str_map);
            free(points);
            return NULL;
        }
        cols = ft_str_strlen(map_row);
        points[row] = malloc(sizeof(t_point) * cols);
        if (!points[row])
        {
            ft_putstr_fd("Couldn't allocate memory for the row\n", 2);
            free_char_arr(str_map);
            free_t_point_arr(points);
        }
        // ft_printf("Map_row length: %d\n", cols);
        col = 0;
        while (col < cols)
        {
            // ft_printf("col %d\n", col);
            char **parts = ft_split(map_row[col], ',');
            points[row][col].x = col;
            points[row][col].y = row;
            points[row][col].z = ft_atoi(parts[0]);
            if (parts[1])
                points[row][col].color = ft_atoi_base(parts[1], 16);
            else
                points[row][col].color = 0xFFFFFF;
            // ft_printf("color %s\n", parts[1]);
            free_char_arr(parts);
            col++;
        }
        free_char_arr(map_row);
        map->rows[row].row = points[row];
        map->rows[row].ncols = cols;
        row++;
    }

    ft_printf("Map has %d rows\n", map->nrows);
    for (int i = 0; i < map->nrows; i++)
    {
        t_row temp_row = map->rows[i];
        for (int j = 0; j < temp_row.ncols; j++)
        {
            ft_printf("X: %d, Y: %d, Z: %d, C: %d\n", temp_row.row[j].x, temp_row.row[j].y, temp_row.row[j].z, temp_row.row[j].color);
            j++;
        }
        ft_printf("This row has %d columns\n", temp_row.ncols);
    }

    return map;
}

int main(int ac, char *av[])
{
    t_win *window;
    t_image *img;
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
    img->width = 900;
    img->height = 600;
    img->img_ptr = mlx_new_image(window->mlx, img->width, img->height);
    if (!img->img_ptr)
    {
        ft_putstr_fd("Couldn't create image\n", 2);
        free(window);
        return 0;
    }

    parse_store_map(fd);
    // draw_put_image(window, img);

    mlx_key_hook(window->win, key_hook, window);
    mlx_hook(window->win, 17, 1L << 17, close_win, window);
    mlx_loop(window->mlx);

    free(window);
    return 0;
}
