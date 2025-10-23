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
    int x;
    int y;
    int z;
    int color;
} t_point;

// utils
int rgb_to_int(double r, double g, double b);
int normalize_y(int curr, int total);
void free_arr(char **arr);

// hooks
int key_hook(int keycode, t_win *vars);
int close_win(int keycode, t_win *vars);

// events

#endif