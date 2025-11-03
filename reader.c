/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:55:43 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 18:03:26 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	safe_open(char *path, int flag)
{
	int	fd;

	fd = open(path, flag);
	if (fd == -1)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

char	*read_file(int fd)
{
	char	*line;
	char	*full_map;
	char	*temp;

	full_map = NULL;
	line = get_next_line(fd);
	while (line != NULL)
	{
		temp = ft_strjoin(full_map, line);
		free(line);
		free(full_map);
		full_map = temp;
		if (full_map == NULL)
		{
			ft_putstr_fd("Couldn't read the map\n", 2);
			close(fd);
			return (NULL);
		}
		line = get_next_line(fd);
	}
	close(fd);
	return (full_map);
}
