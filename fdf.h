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

/*
[
    nrows
    [(point1, ncols1), (point2, ncols2)]
]
 */

// utils
void free_char_arr(char **arr);
void free_t_point_arr(t_point **arr);
void free_map(t_map *map);
size_t ft_str_strlen(char **str);
int largest_row(t_map *map);
char *read_file(int fd);
int height_color(int z, double min_z, double max_z, int color_high);
void apply_isometric(int *x, int *y, int z, int z_scale);

// hooks
int key_hook(int keycode, t_win *vars);
int close_win(int keycode, t_win *vars);

// events

#endif