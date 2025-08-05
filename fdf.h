#ifndef FDF_H
#define FDF_H

#include "Libft/libft.h"
#include "mlx/mlx.h"
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