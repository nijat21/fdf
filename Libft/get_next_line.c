/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:45:49 by nismayil          #+#    #+#             */
/*   Updated: 2025/10/28 20:25:26 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *get_next_line(int fd)
{
	static char buf[BUFFER_SIZE + 1];
	int bytes_read;
	char *nl;

	nl = NULL;
	bytes_read = 0;
	while (bytes_read >= 0)
	{
		if (!buf[0])
		{
			bytes_read = read(fd, buf, BUFFER_SIZE);
			if (bytes_read == 0)
				break;
			if (bytes_read < 0)
			{
				free(nl);
				return (NULL);
			}
			buf[bytes_read] = '\0';
		}
		create_merge(&nl, buf);
		if (!nl || nl[ft_strlen(nl) - 1] == '\n')
			break;
	}
	return (nl);
}
