/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:53:17 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 12:53:17 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "cub3d.h"
#include "cub3d_int.h"

t_c3_vector	c3_get_horizontal_hit_for_index(
	double theta, int index, t_c3_vector *pos)
{
	t_c3_vector	hit;

	if (theta >= 0 && theta < M_PI)
	{
		hit.y = floor(pos->y) + index;
		hit.x = pos->x + (hit.y - pos->y) / tan(theta);
	}
	else
	{
		hit.y = floor(pos->y) - index + 1;
		hit.x = pos->x + (hit.y - pos->y) / tan(theta);
	}
	return (hit);
}

t_c3_vector	c3_get_horizontal_hit_cell_for_index(
	double theta, t_c3_vector *hit)
{
	t_c3_vector	hit_cell;

	if (theta >= 0 && theta < M_PI)
	{
		hit_cell.x = floor(hit->x) + 0.5;
		hit_cell.y = floor(hit->y) + 0.5;
	}
	else
	{
		hit_cell.x = floor(hit->x) + 0.5;
		hit_cell.y = floor(hit->y) - 1 + 0.5;
	}
	return (hit_cell);
}

t_c3_vector	c3_get_vertical_hit_for_index(
	double theta, int index, t_c3_vector *pos)
{
	t_c3_vector	hit;

	if (theta < M_PI_2 || theta >= 3 * M_PI_2)
	{
		hit.x = floor(pos->x) + index;
		hit.y = pos->y + (hit.x - pos->x) * tan(theta);
	}
	else
	{
		hit.x = floor(pos->x) - index + 1;
		hit.y = pos->y + (hit.x - pos->x) * tan(theta);
	}
	return (hit);
}

t_c3_vector	c3_get_vertical_hit_cell_for_index(
	double theta, t_c3_vector *hit)
{
	t_c3_vector	hit_cell;

	if (theta < M_PI_2 || theta >= 3 * M_PI_2)
	{
		hit_cell.x = floor(hit->x) + 0.5;
		hit_cell.y = floor(hit->y) + 0.5;
	}
	else
	{
		hit_cell.x = floor(hit->x) - 1 + 0.5;
		hit_cell.y = floor(hit->y) + 0.5;
	}
	return (hit_cell);
}
