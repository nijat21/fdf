/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:16:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/10/31 00:18:07 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void free_char_arr(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
    arr = NULL;
}

void free_t_point_arr(t_point **arr)
{
    int i;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
    arr = NULL;
}

void free_map(t_map *map)
{
    int i;

    if (!map)
        return;
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

size_t ft_str_strlen(char **str)
{
    size_t count;

    count = 0;
    while (str[count])
        count++;
    return count;
}

int largest_row(t_map *map)
{
    int i;
    int largest;

    i = 0;
    largest = 0;
    while (i < map->nrows)
    {
        if (map->rows[i].ncols > largest)
            largest = map->rows[i].ncols;
        i++;
    }
    return largest;
}

char *read_file(int fd)
{
    char *line;
    char *full_map;
    char *temp;

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
            return (NULL);
        }
        line = get_next_line(fd);
    }
    return (full_map);
}
