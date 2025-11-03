/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:36:05 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 12:15:56 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_point *map_cols(t_map *map, char ***map_row, int row, size_t ncols)
{
    size_t col;
    char **parts;

    col = 0;
    while (col < ncols)
    {
        parts = ft_split((*map_row)[col], ',');
        if (!parts)
        {
            ft_putstr_fd("Couldn't split the map cols\n", 2);
            free_char_arr(*map_row);
            free_map(map);
            return NULL;
        }
        map->rows[row].cols[col].z = ft_atoi(parts[0]);
        if (parts[1])
            map->rows[row].cols[col].color = ft_atoi_base(parts[1], 16);
        else
            map->rows[row].cols[col].color = 0x5727F5;
        free_char_arr(parts);
        col++;
    }
    free_char_arr((*map_row));
    map->rows[row].ncols = ncols;
    return map->rows[row].cols;
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
            ft_putstr_fd("Couldn't read the map\n", 2);
            close(fd);
            return NULL;
        }
        line = get_next_line(fd);
    }
    close(fd);
    return (full_map);
}

t_map *map_init(int fd, char ***str_map, size_t *nrows)
{
    t_map *map;
    char *temp_map;

    temp_map = read_file(fd);
    if (!temp_map)
        return NULL;
    (*str_map) = ft_split(temp_map, '\n');
    free(temp_map);
    if (!(*str_map))
    {
        ft_putstr_fd("Couldn't split the map\n", 2);
        return NULL;
    }
    *nrows = ft_str_strlen((*str_map));
    map = safe_malloc(sizeof(t_map), "Couldn't allocate memory for map\n");
    if (!map)
    {
        free_char_arr(*str_map);
        return NULL;
    }
    map->nrows = *nrows;
    map->rows = safe_malloc(sizeof(t_row) * (*nrows), "Couldn't allocate memory for map->rows\n");
    if (!map->rows)
    {
        free_char_arr(*str_map);
        free_map(map);
        return NULL;
    }
    return map;
}

t_map *parse_store_map(int fd)
{
    char **str_map;
    char **map_row;
    size_t row;
    size_t nrows;
    size_t ncols;
    t_map *map;

    t_point *cols;

    map = map_init(fd, &str_map, &nrows);
    if (!map)
        return NULL;
    row = 0;
    while (row < nrows)
    {
        map_row = ft_split(str_map[row], ' ');
        if (!map_row)
        {
            ft_putstr_fd("Couldn't split the map row\n", 2);
            free_char_arr(str_map);
            free_map(map);
            return NULL;
        }
        ncols = ft_str_strlen(map_row);
        map->rows[row].cols = safe_malloc(sizeof(t_point) * ncols, "Couldn't allocate memory for the row\n");
        if (!map->rows[row].cols)
        {
            free_char_arr(str_map);
            free_char_arr(map_row);
            free_map(map);
            return NULL;
        }
        cols = map_cols(map, &map_row, row, ncols);
        if (!cols)
            return NULL;
        row++;
    }
    free_char_arr(str_map);
    return map;
}
