/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 16:34:13 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 16:34:13 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"
#include "cub3d_int.h"

int		c3_ray_caster_take_hori(
	t_c3_ray_caster *caster, int hori_index, int sprites, t_c3_hit_result *out)
{
	if (caster->hori_hits[hori_index + 1].distance_sqared
		>= caster->wall_distance)
		return (0);
	out[sprites + 1] = caster->hori_hits[hori_index + 1];
	return (1);
}

int		c3_ray_caster_take_vert(
	t_c3_ray_caster *caster, int vert_index, int sprites, t_c3_hit_result *out)
{
	if (caster->vert_hits[vert_index + 1].distance_sqared
		>= caster->wall_distance)
		return (0);
	out[sprites + 1] = caster->vert_hits[vert_index + 1];
	return (1);
}

int		c3_merge_rays_to_sprites(t_c3_ray_caster *cas, t_c3_hit_result *out)
{
	int				hori_index;
	int				vert_index;
	int				sprites;

	hori_index = 0;
	vert_index = 0;
	sprites = 0;
	while (
		sprites < C3_MAX_COLLINEAR_SPRITES
		&& (hori_index < cas->hori_sprites || vert_index < cas->vert_sprites))
		if (hori_index >= cas->hori_sprites && vert_index < cas->vert_sprites)
			sprites += c3_ray_caster_take_vert(cas, vert_index++, sprites, out);
		else if (
			hori_index < cas->hori_sprites && vert_index >= cas->vert_sprites)
			sprites += c3_ray_caster_take_hori(cas, hori_index++, sprites, out);
		else if (
			cas->hori_hits[hori_index + 1].distance_sqared
			< cas->vert_hits[vert_index + 1].distance_sqared)
			sprites += c3_ray_caster_take_hori(cas, hori_index++, sprites, out);
		else
			sprites += c3_ray_caster_take_vert(cas, vert_index++, sprites, out);
	return (sprites);
}

int		c3_cast_ray(
	t_c3_state *stat, t_c3_vector *pos, double theta, t_c3_hit_result *out)
{
	t_c3_ray_caster	caster;
	int				sprites;

	caster.hori_sprites = tan(theta) != 0.0
		? c3_get_horizontal_hit(stat, pos, theta, caster.hori_hits) : 0;
	caster.vert_sprites = c3_get_vertical_hit(
		stat, pos, theta, caster.vert_hits);
	if (tan(theta) != 0.0 &&
		caster.hori_hits[0].distance_sqared
		< caster.vert_hits[0].distance_sqared)
	{
		*out = caster.hori_hits[0];
		caster.wall_distance = caster.hori_hits[0].distance_sqared;
	}
	else
	{
		*out = caster.vert_hits[0];
		caster.wall_distance = caster.vert_hits[0].distance_sqared;
	}
	sprites = c3_merge_rays_to_sprites(&caster, out);
	return (sprites);
}

void	c3_scan(t_c3_state *stat)
{
	int		x;
	int		half_res;
	double	a;
	double	theta;
	double	angle;

	half_res = stat->renderer.resolution_x / 2;
	x = -half_res;
	while (x < half_res)
	{
		a = x * (stat->renderer.plane_distance * tan(stat->renderer.fov))
			/ half_res;
		theta = atan(a / stat->renderer.plane_distance);
		stat->renderer.rays[x + half_res].angle = theta;
		angle = theta + stat->player.direction;
		if (angle >= M_PI * 2)
			angle -= M_PI * 2;
		if (angle < 0)
			angle += M_PI * 2;
		stat->renderer.rays[x + half_res].hit_sprite_count = c3_cast_ray(
			stat, &stat->player.position, angle,
			stat->renderer.rays[x + half_res].hits);
		x++;
	}
}
