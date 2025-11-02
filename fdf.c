/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:18:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/02 01:03:16 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void draw_pixels(t_coordinates coors, t_img_props img_props, t_image *img, t_breh *breh)
{
    while (1)
    {
        if (coors.x1 >= 0 && coors.x1 < img->width && coors.y1 >= 0 && coors.y1 < img->height)
            *(int *)(img_props.data + (coors.y1 * (img_props.size_line) + coors.x1 * (img_props.bpp / 8))) = coors.color;
        if (coors.x1 == coors.x2 && coors.y1 == coors.y2)
            break;
        if (2 * (*breh).e >= (*breh).dy)
        {
            if (coors.x1 == coors.x2)
                break;
            (*breh).e += (*breh).dy;
            coors.x1 += (*breh).sx;
        }
        if (2 * (*breh).e <= (*breh).dx)
        {
            if (coors.y1 == coors.y2)
                break;
            (*breh).e += (*breh).dx;
            coors.y1 += (*breh).sy;
        }
    }
}

// Bresenham's algorithm
void draw_line(t_coordinates coors, t_img_props img_props, t_image *img)
{
    t_breh breh;

    breh.dx = abs(coors.x2 - coors.x1);
    if (coors.x1 < coors.x2)
        breh.sx = 1;
    else
        breh.sx = -1;
    breh.dy = -abs(coors.y2 - coors.y1);
    if (coors.y1 < coors.y2)
        breh.sy = 1;
    else
        breh.sy = -1;
    breh.e = breh.dy + breh.dx;

    draw_pixels(coors, img_props, img, &breh);
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

t_map *map_init(int fd, char ***str_map, size_t *nrows)
{
    t_map *map;
    char *temp_map;

    temp_map = read_file(fd);
    close(fd);
    (*str_map) = ft_split(temp_map, '\n');
    free(temp_map);
    if (!(*str_map))
        handle_error("Couldn't split the map\n", 0);
    *nrows = ft_str_strlen((*str_map));
    map = malloc(sizeof(t_map));
    if (!map)
        handle_error("Couldn't allocate memory for map\n", 1, *str_map, free_char_arr_wrapper);
    map->nrows = *nrows;
    map->rows = safe_malloc(sizeof(t_row) * (*nrows), 2, (*str_map), free_char_arr_wrapper, map, free_map_wrapper);
    return map;
}

void map_cols(t_map *map, char ***map_row, int row, size_t ncols)
{
    size_t col;
    char **parts;

    col = 0;
    while (col < ncols)
    {
        parts = ft_split((*map_row)[col], ',');
        map->rows[row].cols[col].z = ft_atoi(parts[0]);
        if (parts[1])
            map->rows[row].cols[col].color = ft_atoi_base(parts[1], 16);
        else
            map->rows[row].cols[col].color = 0x5727F5;
        free_char_arr(parts);
        col++;
    }
    free_char_arr((*map_row));
    map->rows[row].ncols = ncols;
}

t_map *parse_store_map(int fd)
{
    char **str_map;
    char **map_row;
    size_t row;
    size_t nrows;
    size_t ncols;
    t_map *map;

    map = map_init(fd, &str_map, &nrows);
    row = 0;
    while (row < nrows)
    {
        map_row = ft_split(str_map[row], ' ');
        if (!map_row)
            handle_error("Couldn't split the map row\n", 2, str_map, free_char_arr_wrapper, map, free_map_wrapper);
        ncols = ft_str_strlen(map_row);
        map->rows[row].cols = malloc(sizeof(t_point) * ncols);
        if (!map->rows[row].cols)
            handle_error("Couldn't allocate memory for the row\n", 2, str_map, free_char_arr_wrapper, map, free_map_wrapper);
        map_cols(map, &map_row, row, ncols);
        row++;
    }
    return map;
}

t_win *win_init(void)
{
    t_win *win;

    win = safe_malloc(sizeof(t_win), 0);
    win->mlx = mlx_init();
    win->width = 1920;
    win->height = 1400;
    win->win = mlx_new_window(win->mlx, win->width, win->height, "nismayil's fdf");
    if (!win->win)
    {
        ft_putstr_fd("Couldn't create window\n", 2);
        free(win);
        win = NULL;
        exit(EXIT_FAILURE);
    }
    mlx_clear_window(win->mlx, win->win);
    return win;
}

t_image *img_init(t_win *win)
{
    t_image *img;

    img = safe_malloc(sizeof(t_image), 1, win, free);
    img->width = win->width;
    img->height = win->height;
    img->img_ptr = mlx_new_image(win->mlx, img->width, img->height);
    if (!img->img_ptr)
    {
        ft_putstr_fd("Couldn't create image\n", 2);
        free(img);
        img = NULL;
        free(win);
        win = NULL;
        exit(EXIT_FAILURE);
    }
    return img;
}

int main(int ac, char *av[])
{
    t_win *win;
    t_image *img;
    t_map *map;
    int fd;

    if (ac != 2)
        return 0;
    fd = safe_open(av[1], O_RDONLY);
    win = win_init();
    img = img_init(win);
    map = parse_store_map(fd);
    draw_put_image(win, img, map);
    mlx_key_hook(win->win, key_hook, win);
    mlx_hook(win->win, 17, 1L << 17, close_win, win);
    mlx_loop(win->mlx);
    free(win);
    return 0;
}
