/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:18:38 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/01 23:54:47 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
#define FDF_H

#if defined(__APPLE__)
#include "minilibx_macos_metal/mlx.h"
#elif defined(__linux__)
#include "minilibx-linux/mlx.h"
#else
#error "Unsupported OS"
#endif

#include "Libft/libft.h"
#include <math.h>

typedef struct s_win
{
    void *mlx;
    void *win;
    int height;
    int width;
} t_win;

typedef struct s_image
{
    int height;
    int width;
    void *img_ptr;
} t_image;

typedef struct s_img_props
{
    char *data;
    int bpp;
    int size_line;
    int endian;
} t_img_props;

typedef struct s_img_all
{
    t_image *img;
    t_img_props img_props;
} t_img_all;

typedef struct s_point
{
    int z;
    int color;
} t_point;

typedef struct s_row
{
    t_point *cols;
    int ncols;
} t_row;

typedef struct s_map
{
    int nrows;
    t_row *rows;
} t_map;

typedef struct s_coordinates
{
    int x1;
    int x2;
    int y1;
    int y2;
    int z1;
    int z2;
    int color;
} t_coordinates;

typedef struct s_space
{
    int w_space;
    int h_space;
    int offset_x;
    int offset_y;
} t_space;

typedef struct s_bounds
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int min_z;
    int max_z;
    int z_scale;
} t_bounds;

typedef struct s_breh
{
    int e;
    int sx;
    int dx;
    int sy;
    int dy;
} t_breh;

typedef void (*t_free_func)(void *);

// utils
void free_char_arr(char **arr);
void free_t_point_arr(t_point **arr);
void free_map(t_map *map);
size_t ft_str_strlen(char **str);
void *safe_malloc(size_t size, int n_free, ...);
int safe_open(char *path, int flag);
int largest_row(t_map *map);
char *read_file(int fd);
int height_color(int z, double min_z, double max_z, int color_high);
void apply_isometric(int *x, int *y, int z, int z_scale);
void calculate_offset(t_image *img, t_map *map, t_bounds *bounds, t_space *spaces);
int calculate_z_scale(t_map *map, t_image *img, int *min_z, int *max_z);
void free_char_arr_wrapper(void *p);
void free_t_point_arr_wrapper(void *p);
void free_map_wrapper(void *p);

// hooks
int key_hook(int keycode, t_win *vars);
int close_win(int keycode, t_win *vars);

// events

#endif