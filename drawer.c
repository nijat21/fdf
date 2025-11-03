/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 00:04:12 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 14:57:53 by nismayil         ###   ########.fr       */
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

void draw_put_image(t_win *win, t_image *img, t_map *map)
{
    t_img_props img_props;
    t_space spaces;
    t_bounds bounds;
    t_img_all img_all;

    img_props.data = mlx_get_data_addr(img->img_ptr, &(img_props.bpp), &(img_props.size_line), &(img_props.endian));
    calculate_offset(img, map, &bounds, &spaces);
    img_all.img_props = img_props;
    img_all.img = img;
    horizontal_lines(map, spaces, bounds, img_all);
    vertical_lines(map, spaces, bounds, img_all);
    mlx_put_image_to_window(win->mlx, win->win, img->img_ptr, (win->width - img->width) / 2, (win->height - img->height) / 2);
}
// void draw_put_image(t_win *win, t_image *img, t_map *map)
// {
//     t_img_props img_props;
//     t_space spaces;
//     t_bounds bounds;
//     t_img_all img_all;

//     img_props.data = mlx_get_data_addr(img->img_ptr, &(img_props.bpp), &(img_props.size_line), &(img_props.endian));
//     calculate_offset(img, map, &bounds, &spaces);
//     img_all.img_props = img_props;
//     img_all.img = img;
//     horizontal_lines(map, spaces, bounds, img_all);
//     vertical_lines(map, spaces, bounds, img_all);
//     mlx_put_image_to_window(win->mlx, win->win, img->img_ptr, (win->width - img->width) / 2, (win->height - img->height) / 2);
// }