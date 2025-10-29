#include "fdf.h"

// Bresenham's algorithm
void draw_line(int x1, int y1, int x2, int y2, char *data, int bpp, int size_line, int color, t_image *img)
{
    int dx;
    int dy;
    int e;
    int sx;
    int sy;

    dx = abs(x2 - x1);
    if (x1 < x2)
        sx = 1;
    else
        sx = -1;
    dy = -abs(y2 - y1);
    if (y1 < y2)
        sy = 1;
    else
        sy = -1;
    e = dy + dx;

    while (1)
    {
        if (x1 >= 0 && x1 < img->width && y1 >= 0 && y1 < img->height)
            *(int *)(data + (y1 * size_line + x1 * (bpp / 8))) = color;
        if (x1 == x2 && y1 == y2)
            break;
        if (2 * e >= dy)
        {
            if (x1 == x2)
                break;
            e += dy;
            x1 += sx;
        }
        if (2 * e <= dx)
        {
            if (y1 == y2)
                break;
            e += dx;
            y1 += sy;
        }
    }
}

void apply_isometric(int *x, int *y, int z, int z_scale)
{
    int prev_x;
    int prev_y;

    prev_x = *x;
    prev_y = *y;
    *x = (prev_x - prev_y);
    *y = (prev_x + prev_y) / 2 - z * z_scale;
}

// void apply_isometric(int *x, int *y, int z, int z_scale)
// {
//     int prev_x = *x;
//     int prev_y = *y;
//     int x_rot, y_rot;

//     // 90° CCW rotation
//     x_rot = prev_y;
//     y_rot = -prev_x;

//     /* Apply isometric projection to rotated point */
//     *x = (x_rot - y_rot);
//     *y = (x_rot + y_rot) / 2 - z * z_scale;
// }

/*
    ****  REFACTOR, TEST AND DEBUG
    1. Coloring and gradient by height
    2. Zoom bonus
    3. Rotate bonus
    4. Additional perspective bonus
*/
void get_map_bounds(t_map *map, int w_space, int h_space,
                    int *min_x, int *max_x, int *min_y, int *max_y, int z_scale)
{
    int row = 0;
    int x, y, z;
    int is_first = 1;

    while (row < map->nrows)
    {
        int col = 0;
        while (col < map->rows[row].ncols)
        {
            // Get the point position
            x = col * w_space;
            y = row * h_space;
            z = map->rows[row].cols[col].z;

            // Transform it to isometric
            apply_isometric(&x, &y, z, z_scale);

            // Update min/max values
            if (is_first)
            {
                *min_x = *max_x = x;
                *min_y = *max_y = y;
                is_first = 0;
            }
            else
            {
                if (x < *min_x)
                    *min_x = x;
                if (x > *max_x)
                    *max_x = x;
                if (y < *min_y)
                    *min_y = y;
                if (y > *max_y)
                    *max_y = y;
            }
            col++;
        }
        row++;
    }
}

void calculate_offset(t_image *img, t_map *map, int w_space, int h_space, int *offset_x, int *offset_y, int z_scale)
{
    int min_x, max_x, min_y, max_y;
    int width, height;

    get_map_bounds(map, w_space, h_space, &min_x, &max_x, &min_y, &max_y, z_scale);

    width = max_x - min_x;
    height = max_y - min_y;

    *offset_x = (img->width - width) / 2 - min_x;
    *offset_y = (img->height - height) / 2 - min_y;
}

int calculate_z_scale(t_map *map, t_image *img)
{
    double max_z;
    double min_z;
    double z_scale;
    int row;

    min_z = INFINITY;
    max_z = -INFINITY;
    row = 0;
    while (row < map->nrows)
    {
        int col = 0;
        while (col < map->rows[row].ncols)
        {
            if (map->rows[row].cols[col].z < min_z)
                min_z = map->rows[row].cols[col].z;
            if (map->rows[row].cols[col].z > max_z)
                max_z = map->rows[row].cols[col].z;
            col++;
        }
        row++;
    }
    z_scale = (img->height / 4) / ((max_z - min_z) + 1);
    z_scale = fmax(1, z_scale);
    return (int)z_scale;
}

// additional perspective
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

void draw_put_image(t_win *window, t_image *img, t_map *map)
{
    char *data;
    int bpp;
    int size_line;
    int endian;
    int row;
    int col;
    int offset_x;
    int offset_y;
    int z_scale;

    int x1;
    int y1;
    int z1;

    int x2;
    int y2;
    int z2;

    int w_space = img->width / (largest_row(map) * 2);
    int h_space = img->height / (map->nrows * 2);

    z_scale = calculate_z_scale(map, img);

    data = mlx_get_data_addr(img->img_ptr, &bpp, &size_line, &endian);
    calculate_offset(img, map, w_space, h_space, &offset_x, &offset_y, z_scale);
    // Horizontal connections
    row = 0;
    while (row < map->nrows)
    {
        col = 0;
        while (col < map->rows[row].ncols - 1)
        {
            x1 = col * w_space;
            y1 = row * h_space;
            z1 = map->rows[row].cols[col].z;

            x2 = (col + 1) * w_space;
            y2 = row * h_space;
            z2 = map->rows[row].cols[col + 1].z;

            apply_isometric(&x1, &y1, z1, z_scale);
            apply_isometric(&x2, &y2, z2, z_scale);
            // paralel_perspective(&x1, &y1, window, img);
            // paralel_perspective(&x2, &y2, window, img);

            draw_line(
                x1 + offset_x,
                y1 + offset_y,
                x2 + offset_x,
                y2 + offset_y,
                data, bpp, size_line, map->rows[row].cols[col].color, img);
            col++;
        }
        row++;
    }
    // Vertical connections
    row = 0;
    while (row < map->nrows - 1)
    {
        col = 0;
        while (col < map->rows[row].ncols && col < map->rows[row + 1].ncols)
        {
            x1 = col * w_space;
            y1 = row * h_space;
            z1 = map->rows[row].cols[col].z;

            x2 = col * w_space;
            y2 = (row + 1) * h_space;
            z2 = map->rows[row + 1].cols[col].z;

            apply_isometric(&x1, &y1, z1, z_scale);
            apply_isometric(&x2, &y2, z2, z_scale);
            // paralel_perspective(&x1, &y1, window, img);
            // paralel_perspective(&x2, &y2, window, img);

            draw_line(
                x1 + offset_x,
                y1 + offset_y,
                x2 + offset_x,
                y2 + offset_y,
                data, bpp, size_line, map->rows[row].cols[col].color, img);
            col++;
        }
        row++;
    }
    mlx_put_image_to_window(window->mlx, window->win, img->img_ptr, (window->width - img->width) / 2, (window->height - img->height) / 2);
}

t_map *parse_store_map(int fd)
{
    char *temp_map;
    char **str_map;
    char **map_row;
    char **parts;
    size_t row;
    size_t nrows;
    size_t col;
    size_t ncols;
    t_map *map;

    temp_map = read_file(fd);
    close(fd);
    if (!temp_map)
    {
        ft_putstr_fd("Couldn't read the map\n", 2);
        return NULL;
    }
    str_map = ft_split(temp_map, '\n');
    free(temp_map);
    if (!str_map)
    {
        ft_putstr_fd("Couldn't split the map\n", 2);
        return NULL;
    }
    nrows = ft_str_strlen(str_map);
    map = malloc(sizeof(t_map));
    if (!map)
    {
        ft_putstr_fd("Couldn't allocate memory for map\n", 2);
        free_char_arr(str_map);
        return NULL;
    }
    map->nrows = nrows;
    map->rows = malloc(sizeof(t_row) * nrows);
    if (!map->rows)
    {
        ft_putstr_fd("Couldn't allocate memory for map->rows\n", 2);
        free_char_arr(str_map);
        free_map(map);
        return NULL;
    }
    row = 0;
    while (row < nrows)
    {
        // ft_printf("row %d\n", row);
        map_row = ft_split(str_map[row], ' ');
        if (!map_row)
        {
            ft_putstr_fd("Couldn't split the map row\n", 2);
            free_char_arr(str_map);
            free_map(map);
            return NULL;
        }
        ncols = ft_str_strlen(map_row);
        map->rows[row].cols = malloc(sizeof(t_point) * ncols);
        if (!map->rows[row].cols)
        {
            ft_putstr_fd("Couldn't allocate memory for the row\n", 2);
            free_char_arr(str_map);
            free_map(map);
            return NULL;
        }
        col = 0;
        while (col < ncols)
        {
            parts = ft_split(map_row[col], ',');
            map->rows[row].cols[col].z = ft_atoi(parts[0]);
            if (parts[1])
                map->rows[row].cols[col].color = ft_atoi_base(parts[1], 16);
            else
                map->rows[row].cols[col].color = 0x69F5D5;
            free_char_arr(parts);
            col++;
        }
        free_char_arr(map_row);
        map->rows[row].ncols = ncols;
        row++;
    }

    // ft_printf("Map has %d rows\n", map->nrows);
    // for (int i = 0; i < map->nrows; i++)
    // {
    //     t_row temp_row = map->rows[i];
    //     for (int j = 0; j < temp_row.ncols; j++)
    //         ft_printf("Z: %d, C: %d\n", temp_row.cols[j].z, temp_row.cols[j].color);
    //     ft_printf("This row has %d columns\n", temp_row.ncols);
    // }

    return map;
}

int main(int ac, char *av[])
{
    t_win *window;
    t_image *img;
    t_map *map;
    int fd;

    if (ac != 2)
        return 0;
    fd = open(av[1], O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return 0;
    }

    window = malloc(sizeof(t_win));
    if (!window)
        return 0;

    window->mlx = mlx_init();
    window->width = 1920;
    window->height = 1400;
    window->win = mlx_new_window(window->mlx, window->width, window->height, "nismayil's fdf");

    // Clear the window before drawing
    mlx_clear_window(window->mlx, window->win);

    // Create an image
    img = malloc(sizeof(t_image));
    if (!img)
    {
        free(window);
        return 0;
    }
    img->width = window->width;
    img->height = window->height;
    img->img_ptr = mlx_new_image(window->mlx, img->width, img->height);
    if (!img->img_ptr)
    {
        ft_putstr_fd("Couldn't create image\n", 2);
        free(window);
        return 0;
    }

    map = parse_store_map(fd);
    draw_put_image(window, img, map);

    mlx_key_hook(window->win, key_hook, window);
    mlx_hook(window->win, 17, 1L << 17, close_win, window);
    mlx_loop(window->mlx);

    free(window);
    return 0;
}
