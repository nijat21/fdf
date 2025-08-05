#include "fdf.h"

/* If ESC cliced, close the window in a clean way */
/* In MacOS, window is automatically closed when X button is clicked.
In Linux, it should be handled manually */
int key_hook(int keycode, t_vars *vars)
{
    ft_printf("keycode: %d\n", keycode);
    switch (keycode)
    {
    case 53:
    case 65307:
        mlx_destroy_window(vars->mlx, vars->win);
        exit(0);
        break;
    }
    return (0);
}

int close_win(int keycode, t_vars *vars)
{
    (void)keycode;
#if defined(__linux__)
    mlx_destroy_window(vars->mlx, vars->win);
#endif
    (void)vars;
    exit(0);
    return 0;
}