/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:49:39 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 12:49:39 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "cub3d.h"
#include "cub3d_int.h"

int		c3_add_sprite(
	t_c3_vector *hit, t_c3_vector *hit_cell, t_c3_vector *pos,
	t_c3_hit_result *result)
{
	double		dot;
	t_c3_vector	ad;
	double		c;
	double		offset;

	dot = c3_dot(pos, hit_cell, hit);
	ad.x = hit->x - pos->x;
	ad.y = hit->y - pos->y;
	c = result[1].distance_sqared / dot;
	hit->x = pos->x + ad.x * c;
	hit->y = pos->y + ad.y * c;
	offset = sqrt(c3_distance_squared(hit, hit_cell));
	if (offset <= 0.5)
	{
		t_c3_vector	ab = {hit_cell->x - pos->x, hit_cell->y - pos->y};
		double	cross = ab.x * ad.y - ad.x * ab.y;
		if (cross > 0)
			offset = - offset;
		result[1].position = *hit;
		result[1].offset = offset;
		return (1);
	}
	return (0);
}

int		c3_check_wall(t_c3_state *stat, t_c3_vector *hit)
{
	if (hit->x < 0 || hit->x >= stat->map.width
		|| hit->y < 0 || hit->y >= stat->map.height)
		return (1);
	if (c3_query_map(stat, hit->x, hit->y) == C3_MAP_SYMBOL_WALL)
		return (1);
	return (0);
}

int		c3_check_sprite(
	t_c3_state *stat, t_c3_vector *center,
	t_c3_vector *pos, t_c3_hit_result *result)
{
	if (c3_query_map(stat, center->x, center->y) == C3_MAP_SYMBOL_SPRITE)
	{
		result->type = C3_MAP_SYMBOL_SPRITE;
		result->distance_sqared = c3_distance_squared(pos, center);
		return (1);
	}
	return (0);
}

double	c3_dot(t_c3_vector *origin, t_c3_vector *a, t_c3_vector *b)
{
	return (a->x - origin->x) * (b->x - origin->x)
		+ (a->y - origin->y) * (b->y - origin->y);
}

int		c3_get_horizontal_hit(
	t_c3_state *stat, t_c3_vector *pos,
	double theta, t_c3_hit_result *result)
{
	t_c3_vector	hit;
	t_c3_vector	hit_cell;
	int			facing_north;
	int			index;
	int			hit_sprites;

	hit_sprites = 0;
	index = 1;
	while (1)
	{
		facing_north = !(theta >= 0 && theta < M_PI);
		hit = c3_get_horizontal_hit_for_index(theta, index, pos);
		hit_cell = c3_get_horizontal_hit_cell_for_index(theta, &hit);
		if (c3_check_wall(stat, &hit_cell))
			break ;
		if (hit_sprites < C3_MAX_COLLINEAR_SPRITES
			&& c3_check_sprite(stat, &hit_cell, pos, &result[hit_sprites + 1]))
			if(c3_add_sprite(&hit, &hit_cell, pos, result + hit_sprites))
				hit_sprites ++;
		index++;
	}
	result->position = hit;
	result->distance_sqared = c3_distance_squared(pos, &hit);
	result->type = facing_north ? C3_OBJTYPE_WALL_N : C3_OBJTYPE_WALL_S;
	return (hit_sprites);
}

int		c3_get_vertical_hit(
	t_c3_state *stat, t_c3_vector *pos,
	double theta, t_c3_hit_result *result)
{
	t_c3_vector	hit;
	t_c3_vector	hit_cell;
	int			facing_east;
	int			index;
	int			hit_sprites;

	hit_sprites = 0;
	index = 1;
	while (1)
	{
		facing_east = (theta < M_PI_2 || theta >= 3 * M_PI_2);
		hit = c3_get_vertical_hit_for_index(theta, index, pos);
		hit_cell = c3_get_vertical_hit_cell_for_index(theta, &hit);
		if (c3_check_wall(stat, &hit_cell))
			break ;
		if (hit_sprites < C3_MAX_COLLINEAR_SPRITES
			&& c3_check_sprite(stat, &hit_cell, pos, &result[hit_sprites + 1]))
			if(c3_add_sprite(&hit, &hit_cell, pos, result + hit_sprites))
				hit_sprites ++;
		index++;
	}
	result->position = hit;
	result->distance_sqared = c3_distance_squared(pos, &hit);
	result->type = facing_east ? C3_OBJTYPE_WALL_E : C3_OBJTYPE_WALL_W;
	return (hit_sprites);
}
