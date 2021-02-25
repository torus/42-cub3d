/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 16:38:38 by thisai            #+#    #+#             */
/*   Updated: 2021/02/25 16:38:38 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

unsigned int	c3_wall_texel(
	t_c3_state *stat, t_c3_ray *ray, int wall_height, int y)
{
	unsigned int	col;
	double			v;

	v = (y - (stat->renderer.resolution_y - wall_height) / 2.0) / wall_height;
	if (ray->hits[0].type == C3_OBJTYPE_WALL_N)
		col = c3_sample_texture(
			stat, C3_OBJTYPE_WALL_N, fmod(ray->hits[0].position.x, 1.0), v);
	else if (ray->hits[0].type == C3_OBJTYPE_WALL_E)
		col = c3_sample_texture(
			stat, C3_OBJTYPE_WALL_E, fmod(ray->hits[0].position.y, 1.0), v);
	else if (ray->hits[0].type == C3_OBJTYPE_WALL_S)
		col = c3_sample_texture(
			stat, C3_OBJTYPE_WALL_S,
			1.0 - fmod(ray->hits[0].position.x, 1.0), v);
	else
		col = c3_sample_texture(
			stat, C3_OBJTYPE_WALL_W,
			1.0 - fmod(ray->hits[0].position.y, 1.0), v);
	return (col);
}

int				c3_render_test_sprite(
	t_c3_state *stat, t_c3_ray *ray, unsigned int *col, int y)
{
	int		i;
	double	distance;
	int		height;
	double	v;
	double	u;

	i = 0;
	while (i < ray->hit_sprite_count)
	{
		distance = sqrt(ray->hits[i + 1].distance_sqared);
		height = stat->renderer.resolution_y / distance;
		if (y < (stat->renderer.resolution_y - height) / 2.0
			|| y >= (stat->renderer.resolution_y + height) / 2.0)
			break ;
		v = (y - (stat->renderer.resolution_y - height) / 2.0) / height;
		u = 0.5 + ray->hits[i + 1].offset * ((int)(distance * 5) % 2 ? -1 : 1);
		if (u >= 0 && u < 1)
		{
			*col = c3_sample_texture(stat, C3_OBJTYPE_SPRITE, u, v);
			if (*col != C3_TRANSPARENT_COLOR)
				return (1);
		}
		i++;
	}
	return (0);
}

void			c3_render_fill_pixel(
	t_c3_state *stat, int x, int y, uint32_t col)
{
	int	screen_y;
	int	screen_x;
	int	index;

	screen_y = y * stat->screen_height / stat->renderer.resolution_y;
	while (
		screen_y < (y + 1) * stat->screen_height / stat->renderer.resolution_y)
	{
		screen_x = stat->screen_width * x / stat->renderer.resolution_x;
		while (
			screen_x <
			stat->screen_width * (x + 1) / stat->renderer.resolution_x)
		{
			index =
				screen_y * stat->imgdata.size_line +
				screen_x * stat->imgdata.bits_per_pixel / 8;
			((uint32_t*)stat->imgdata.data)[index / 4] = col;
			screen_x++;
		}
		screen_y++;
	}
}

uint32_t		c3_render_get_wall_pixel(
	t_c3_state *stat, int wall_height, int y, t_c3_ray *ray)
{
	uint32_t	col;

	if (y < (stat->renderer.resolution_y - wall_height) / 2.0)
		col = mlx_get_color_value(
			stat->mlx, stat->renderer.ceiling_color);
	else if (y >= (stat->renderer.resolution_y + wall_height) / 2.0)
		col = mlx_get_color_value(
			stat->mlx, stat->renderer.floor_color);
	else
		col = c3_wall_texel(stat, ray, wall_height, y);
	return (col);
}

void			c3_render_scene(t_c3_state *stat)
{
	int				x;
	int				y;
	t_c3_ray		*ray;
	unsigned int	col;
	int				wall_height;

	x = 0;
	while (x < stat->renderer.resolution_x)
	{
		ray = &stat->renderer.rays[x];
		wall_height = stat->renderer.resolution_y
			/ (sqrt(ray->hits[0].distance_sqared) * cos(ray->angle));
		y = 0;
		while (y < stat->renderer.resolution_y)
		{
			if (!c3_render_test_sprite(stat, ray, &col, y))
				col = c3_render_get_wall_pixel(stat, wall_height, y, ray);
			c3_render_fill_pixel(stat, x, y, col);
			y++;
		}
		x++;
	}
}
