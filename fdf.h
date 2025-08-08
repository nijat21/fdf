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

typedef struct s_vars
{
    void *mlx;
    void *win;
} t_vars;

// utils
int rgb_to_int(double r, double g, double b);
int normalize_y(int curr, int total);

// hooks
int key_hook(int keycode, t_vars *vars);
int close_win(int keycode, t_vars *vars);

// events

#endif