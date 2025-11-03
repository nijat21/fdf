/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:18:30 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 20:07:53 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	key_hook(int keycode, t_win *win)
{
	if (keycode == 53 || keycode == 65307)
	{
		mlx_destroy_window(win->mlx, win->win);
		free(win);
		exit(0);
	}
	return (0);
}

int	close_win(t_win *win)
{
	mlx_destroy_window(win->mlx, win->win);
	free(win);
	exit(0);
	return (0);
}
