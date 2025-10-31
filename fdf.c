/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:18:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/10/31 00:58:03 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Bresenham's algorithm
void draw_line(t_coordinates coors, t_img_props img_props, t_image *img)
{
    int dx;
    int dy;
    int e;
    int sx;
    int sy;

    dx = abs(coors.x2 - coors.x1);
    if (coors.x1 < coors.x2)
        sx = 1;
    else
        sx = -1;
    dy = -abs(coors.y2 - coors.y1);
    if (coors.y1 < coors.y2)
        sy = 1;
    else
        sy = -1;
    e = dy + dx;

    while (1)
    {
        if (coors.x1 >= 0 && coors.x1 < img->width && coors.y1 >= 0 && coors.y1 < img->height)
            *(int *)(img_props.data + (coors.y1 * (img_props.size_line) + coors.x1 * (img_props.bpp / 8))) = coors.color;
        if (coors.x1 == coors.x2 && coors.y1 == coors.y2)
            break;
        if (2 * e >= dy)
        {
            if (coors.x1 == coors.x2)
                break;
            e += dy;
            coors.x1 += sx;
        }
        if (2 * e <= dx)
        {
            if (coors.y1 == coors.y2)
                break;
            e += dx;
            coors.y1 += sy;
        }
    }
}

/*
****  REFACTOR, TEST AND DEBUG
****  BUFFERING
****  PROXIMATION TO THE SAMPLE FDF_LINUX
*/

int calculate_z_scale(t_map *map, t_image *img, int *min_z, int *max_z)
{
    double max;
    double min;
    double z_scale;
    int row;

    min = INFINITY;
    max = -INFINITY;
    row = 0;
    while (row < map->nrows)
    {
        int col = 0;
        while (col < map->rows[row].ncols)
        {
            if (map->rows[row].cols[col].z < min)
                min = map->rows[row].cols[col].z;
            if (map->rows[row].cols[col].z > max)
                max = map->rows[row].cols[col].z;
            col++;
        }
        row++;
    }
    *min_z = min;
    *max_z = max;
    z_scale = (img->height / 6) / ((max - min) + 1);
    return (int)fmax(3, z_scale);
}

void apply_offset(t_coordinates *coors, t_space *spaces)
{
    (*coors).x1 += (*spaces).offset_x;
    (*coors).x2 += (*spaces).offset_x;
    (*coors).y1 += (*spaces).offset_y;
    (*coors).y2 += (*spaces).offset_y;
}

void horizontal_lines(t_map *map, t_space spaces, t_bounds bounds, t_img_all img_all)
{
    int row;
    int col;
    t_coordinates coors;

    row = 0;
    while (row < map->nrows)
    {
        col = 0;
        while (col < map->rows[row].ncols - 1)
        {
            coors.x1 = col * spaces.w_space;
            coors.y1 = row * spaces.h_space;
            coors.z1 = map->rows[row].cols[col].z;
            coors.x2 = (col + 1) * spaces.w_space;
            coors.y2 = row * spaces.h_space;
            coors.z2 = map->rows[row].cols[col + 1].z;
            apply_isometric(&coors.x1, &coors.y1, coors.z1, bounds.z_scale);
            apply_isometric(&coors.x2, &coors.y2, coors.z2, bounds.z_scale);
            apply_offset(&coors, &spaces);
            coors.color = height_color((coors.z1 + coors.z2) / 2, bounds.min_z, bounds.max_z, map->rows[row].cols[col].color);
            draw_line(coors, img_all.img_props, img_all.img);
            col++;
        }
        row++;
    }
}

void vertical_lines(t_map *map, t_space spaces, t_bounds bounds, t_img_all img_all)
{
    int row;
    int col;
    t_coordinates coors;

    row = 0;
    while (row < map->nrows - 1)
    {
        col = 0;
        while (col < map->rows[row].ncols && col < map->rows[row + 1].ncols)
        {
            coors.x1 = col * spaces.w_space;
            coors.y1 = row * spaces.h_space;
            coors.z1 = map->rows[row].cols[col].z;
            coors.x2 = col * spaces.w_space;
            coors.y2 = (row + 1) * spaces.h_space;
            coors.z2 = map->rows[row + 1].cols[col].z;
            apply_isometric(&coors.x1, &coors.y1, coors.z1, bounds.z_scale);
            apply_isometric(&coors.x2, &coors.y2, coors.z2, bounds.z_scale);
            apply_offset(&coors, &spaces);
            coors.color = height_color((coors.z1 + coors.z2) / 2, bounds.min_z, bounds.max_z, map->rows[row].cols[col].color);
            draw_line(coors, img_all.img_props, img_all.img);
            col++;
        }
        row++;
    }
}

void draw_put_image(t_win *window, t_image *img, t_map *map)
{
    t_img_props img_props;
    t_space spaces;
    t_bounds bounds;
    t_img_all img_all;

    spaces.w_space = (int)fmax(2, (img->width) / (largest_row(map) * 2));
    spaces.h_space = (int)fmax(2, (img->height) / (map->nrows * 2));
    img_props.data = mlx_get_data_addr(img->img_ptr, &(img_props.bpp), &(img_props.size_line), &(img_props.endian));
    calculate_offset(img, map, &bounds, &spaces);
    img_all.img_props = img_props;
    img_all.img = img;
    horizontal_lines(map, spaces, bounds, img_all);
    vertical_lines(map, spaces, bounds, img_all);
    mlx_put_image_to_window(window->mlx, window->win, img->img_ptr, (window->width - img->width) / 2, (window->height - img->height) / 2);
}

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
            map->rows[row].cols[col].z = ft_atoi(parts[0]);
            if (parts[1])
                map->rows[row].cols[col].color = ft_atoi_base(parts[1], 16);
            else
                map->rows[row].cols[col].color = 0x5727F5;
            free_char_arr(parts);
            col++;
        }
        free_char_arr(map_row);
        map->rows[row].ncols = ncols;
        row++;
    }
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
    img->width = window->width;
    img->height = window->height;
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
