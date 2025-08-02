#ifndef FDF_H
#define FDF_H

#include "Libft/libft.h"
#include "mlx/mlx.h"
#include <math.h>

typedef struct s_win
{
    void *mlx;
    void *win;
} t_win;

int rgb_to_int(double r, double g, double b);

#endif