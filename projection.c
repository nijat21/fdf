#include "fdf.h"

// BONUNSES
void paralel_perspective(int *x, int *y, t_win *window, t_image *img)
{
    int iso_x;
    int iso_y;
    float depth;

    iso_x = *x;
    iso_y = *y;
    depth = 2000.0f / (2000.0f - iso_y);
    *x = (window->width - img->width) / 2 + iso_x * depth;
    *y = (window->height - img->height) / 2 + iso_y * depth;
}

int height_color(int z, double min_z, double max_z, int color_high)
{
    double t;
    int color_low;
    int r;
    int g;
    int b;

    if (color_high != 0x69F5D5)
        return color_high;

    t = z / (max_z - min_z);
    t = t * t * (3.0 - 2.0 * t);
    color_low = 0x5727F5;
    r = (int)(((color_low >> 16) & 0xFF) + t * (((color_high >> 16) & 0xFF) - ((color_low >> 16) & 0xFF)));
    g = (int)(((color_low >> 8) & 0xFF) + t * (((color_high >> 8) & 0xFF) - ((color_low >> 8) & 0xFF)));
    b = (int)((color_low & 0xFF) + t * ((color_high & 0xFF) - (color_low & 0xFF)));

    return (r << 16) | (g << 8) | b;
}

void apply_isometric(int *x, int *y, int z, int z_scale)
{
    int prev_x = *x;
    int prev_y = *y;
    int x_rot;
    int y_rot;

    // 90° CCW rotation
    // x_rot = prev_y;
    // y_rot = -prev_x;
    x_rot = prev_x;
    y_rot = prev_y;

    /* Apply isometric projection to rotated point */
    // *x = (x_rot - y_rot);
    // *y = (x_rot + y_rot) / 2 - z * z_scale;
    *x = (x_rot - y_rot) * cos(0.523599);
    *y = (x_rot + y_rot) * sin(0.523599) - z * z_scale;
}
