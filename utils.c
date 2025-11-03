/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:16:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 12:13:26 by nismayil         ###   ########.fr       */
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

void exit_msg(char *msg)
{
    if (msg)
        ft_putstr_fd(msg, 2);
    exit(EXIT_FAILURE);
}

int safe_open(char *path, int flag)
{
    int fd;

    fd = open(path, flag);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void *safe_malloc(size_t size, char *err_msg)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr)
    {
        perror(err_msg);
        return NULL;
    }
    return ptr;
}
