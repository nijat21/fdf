/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:46 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 20:06:22 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_char_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_t_point_arr(t_point **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_map(t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->rows)
	{
		i = 0;
		while (i < map->nrows)
		{
			free(map->rows[i].cols);
			map->rows[i].cols = NULL;
			i++;
		}
		free(map->rows);
		map->rows = NULL;
	}
	free(map);
	map = NULL;
}
