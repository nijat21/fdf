/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:16:30 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 03:01:08 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void apply_offset(t_coordinates *coors, t_space *spaces)
{
    (*coors).x1 += (*spaces).offset_x;
    (*coors).x2 += (*spaces).offset_x;
    (*coors).y1 += (*spaces).offset_y;
    (*coors).y2 += (*spaces).offset_y;
}

void apply_isometric(int *x, int *y, int z, int z_scale)
{
    int prev_x;
    int prev_y;

    prev_x = *x;
    prev_y = *y;

    *x = (prev_x - prev_y) * 173 / 200;
    *y = (prev_x + prev_y) * 100 / 200 - z * z_scale;
}
