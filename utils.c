/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:16:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 18:01:21 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

size_t	ft_str_strlen(char **str)
{
	size_t	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}

int	largest_row(t_map *map)
{
	int	i;
	int	largest;

	i = 0;
	largest = 0;
	while (i < map->nrows)
	{
		if (map->rows[i].ncols > largest)
			largest = map->rows[i].ncols;
		i++;
	}
	return (largest);
}

void	exit_msg(char *msg)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

void	*safe_malloc(size_t size, char *err_msg)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror(err_msg);
		return (NULL);
	}
	return (ptr);
}
