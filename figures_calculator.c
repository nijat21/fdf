/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   figures_calculator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 00:06:30 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 02:59:39 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int calculate_z_scale(t_map *map, t_image *img, int *min_z, int *max_z)
{
    double max;
    double min;
    double z_scale;
    int row;

    min = LOCAL_INT_MAX;
    max = LOCAL_INT_MIN;
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

int height_color(int z, double min_z, double max_z, int color)
{
    double t;
    int color_high;
    int r;
    int g;
    int b;

    if (color != 0x5727F5)
        return color;

    t = (z - min_z) / (max_z - min_z);
    t = t * t * (3.0 - 2.0 * t);
    t *= 0.8;
    if (t < 0)
        t = 0;
    else if (t > 1)
        t = 1;
    color_high = 0x2BFFB1;
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
    r = r + (int)(t * (((color_high >> 16) & 0xFF) - r));
    g = g + (int)(t * (((color_high >> 8) & 0xFF) - g));
    b = b + (int)(t * ((color_high & 0xFF) - b));

    return (r << 16) | (g << 8) | b;
}

void xy_bounds(int x, int y, t_bounds *bounds)
{
    if (x < (*bounds).min_x)
        (*bounds).min_x = x;
    if (x > (*bounds).max_x)
        (*bounds).max_x = x;
    if (y < (*bounds).min_y)
        (*bounds).min_y = y;
    if (y > (*bounds).max_y)
        (*bounds).max_y = y;
}

void get_map_bounds(t_map *map, int w_space, int h_space, t_bounds *bounds)
{
    int row;
    int col;
    int x;
    int y;
    int z;

    row = 0;
    while (row < map->nrows)
    {
        col = 0;
        while (col < map->rows[row].ncols)
        {
            x = col * w_space;
            y = row * h_space;
            z = map->rows[row].cols[col].z;
            apply_isometric(&x, &y, z, (*bounds).z_scale);
            xy_bounds(x, y, bounds);
            col++;
        }
        row++;
    }
}

void calculate_offset(t_image *img, t_map *map, t_bounds *bounds, t_space *spaces)
{
    int width;
    int height;
    int min_z;
    int max_z;

    (*spaces).w_space = (int)fmax(2, (img->width) / (largest_row(map) * 2));
    (*spaces).h_space = (int)fmax(2, (img->height) / (map->nrows * 2));
    (*bounds).z_scale = calculate_z_scale(map, img, &min_z, &max_z);
    (*bounds).min_x = LOCAL_INT_MAX;
    (*bounds).max_x = LOCAL_INT_MIN;
    (*bounds).min_y = LOCAL_INT_MAX;
    (*bounds).max_y = LOCAL_INT_MIN;
    (*bounds).min_z = min_z;
    (*bounds).max_z = max_z;
    get_map_bounds(map, (*spaces).w_space, (*spaces).h_space, bounds);
    width = (*bounds).max_x - (*bounds).min_x;
    height = (*bounds).max_y - (*bounds).min_y;
    (*spaces).offset_x = (img->width - width) / 2 - (*bounds).min_x;
    (*spaces).offset_y = (img->height - height) / 2 - (*bounds).min_y;
}