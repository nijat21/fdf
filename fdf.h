/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:18:38 by nismayil          #+#   #+#             */
/*   Updated: 2025/11/03 18:22:45 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# if defined(__APPLE__)
#  include "minilibx_macos_metal/mlx.h"
# elif defined(__linux__)
#  include "minilibx-linux/mlx.h"
# else
#  error "Unsupported OS"
# endif

# include "Libft/libft.h"
# include <math.h>

# define LOCAL_INT_MAX 2147483647
# define LOCAL_INT_MIN -2147483648

typedef struct s_win
{
	void		*mlx;
	void		*win;
	int			height;
	int			width;
}				t_win;

typedef struct s_image
{
	int			height;
	int			width;
	void		*img_ptr;
}				t_image;

typedef struct s_img_props
{
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
}				t_iprops;

typedef struct s_img_all
{
	t_image		*img;
	t_iprops	img_props;
}				t_img_all;

typedef struct s_point
{
	int			z;
	int			color;
}				t_point;

typedef struct s_row
{
	t_point		*cols;
	int			ncols;
}				t_row;

typedef struct s_map
{
	int			nrows;
	t_row		*rows;
}				t_map;

typedef struct s_coordinates
{
	int			x1;
	int			x2;
	int			y1;
	int			y2;
	int			z1;
	int			z2;
	int			color;
}				t_coors;

typedef struct s_space
{
	int			w_space;
	int			h_space;
	int			offset_x;
	int			offset_y;
}				t_space;

typedef struct s_bounds
{
	int			min_x;
	int			min_y;
	int			max_x;
	int			max_y;
	int			min_z;
	int			max_z;
	double		z_scale;
}				t_bounds;

typedef struct s_breh
{
	int			e;
	int			sx;
	double		dx;
	int			sy;
	double		dy;
}				t_breh;

// utils
size_t			ft_str_strlen(char **str);
int				largest_row(t_map *map);
int				height_color(int z, double min_z, double max_z, int color_high);
void			exit_msg(char *msg);
void			*safe_malloc(size_t size, char *err_msg);

// Reader
int				safe_open(char *path, int flag);
char			*read_file(int fd);

// Freer
void			free_char_arr(char **arr);
void			free_t_point_arr(t_point **arr);
void			free_map(t_map *map);

// Projection
void			point_apply_isometric(int *x, int *y, int z, double z_scale);
void			apply_isometric(t_coors *coors, t_bounds *bounds);
void			apply_offset(t_coors *coors, t_space *spaces);

// Parser
t_map			*parse_store_map(int fd);

// Calculators
double			calculate_z_scale(t_map *map, t_image *img, int *min_z,
					int *max_z);
int				height_color(int z, double min_z, double max_z, int color);
void			calculate_offset(t_image *img, t_map *map, t_bounds *bounds,
					t_space *spaces);

// Drawer
void			draw_put_image(t_win *win, t_image *img, t_map *map);

// hooks
int				key_hook(int keycode, t_win *vars);
int				close_win(t_win *vars);

#endif
