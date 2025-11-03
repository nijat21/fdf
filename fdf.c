/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:18:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 13:00:09 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
****  REFACTOR, TEST AND DEBUG
****  BUFFERING
****  PROXIMATION TO THE SAMPLE FDF_LINUX
*/

t_image	*img_init(t_win *win, int fd)
{
	t_image	*img;

	img = malloc(sizeof(t_image));
	if (!img)
	{
		close(fd);
		free(win);
		exit_msg("Image couldn't be allocated\n");
	}
	img->width = win->width;
	img->height = win->height;
	img->img_ptr = mlx_new_image(win->mlx, img->width, img->height);
	if (!img->img_ptr)
	{
		close(fd);
		free(win);
		free(img);
		exit_msg("Couldn't create image\n");
	}
	return (img);
}

t_win	*win_init(int fd)
{
	t_win	*win;

	win = malloc(sizeof(t_win));
	if (!win)
	{
		close(fd);
		exit_msg("Window couldn't be allocated\n");
	}
	win->mlx = mlx_init();
	if (!win->mlx)
	{
		close(fd);
		exit_msg("Issue with initiating Mlx\n");
	}
	win->width = 1920;
	win->height = 1400;
	win->win = mlx_new_window(win->mlx, win->width, win->height,
			"nismayil's fdf");
	if (!win->win)
	{
		close(fd);
		exit_msg("Couldn't create window\n");
	}
	mlx_clear_window(win->mlx, win->win);
	return (win);
}

int	main(int ac, char *av[])
{
	t_win	*win;
	t_image	*img;
	t_map	*map;
	int		fd;

	if (ac != 2)
		return (0);
	fd = safe_open(av[1], O_RDONLY);
	win = win_init(fd);
	img = img_init(win, fd);
	map = parse_store_map(fd);
	if (!map)
	{
		free(img);
		free(win);
		return (0);
	}
	draw_put_image(win, img, map);
	mlx_key_hook(win->win, key_hook, win);
	mlx_hook(win->win, 17, 1L << 17, close_win, win);
	mlx_loop(win->mlx);
	return (0);
}
