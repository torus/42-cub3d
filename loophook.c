/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loophook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:24:40 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 12:24:40 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

void	c3_draw(t_c3_state *stat)
{
	c3_render_scene(stat);
	if (stat->is_drawing_minimap)
	{
		c3_draw_map(stat);
		c3_draw_rays_on_map(stat);
	}
	if (stat->is_showing_screen)
		mlx_put_image_to_window(stat->mlx, stat->window, stat->img, 0, 0);
}

typedef struct	s_c3_ray_caster
{
	t_c3_hit_result	hori_hits[1 + C3_MAX_COLLINEAR_SPRITES];
	t_c3_hit_result	vert_hits[1 + C3_MAX_COLLINEAR_SPRITES];
	int				hori_sprites;
	int				vert_sprites;
	/* int				hori_index; */
	/* int				vert_index; */
	/* int				sprites; */
	double			wall_distance;
}				t_c3_ray_caster;

int		c3_cast_ray_to_sprites(t_c3_ray_caster *caster, t_c3_hit_result *out)
{
	int				hori_index;
	int				vert_index;
	int				sprites;

	hori_index = 0;
	vert_index = 0;
	sprites = 0;
	while (sprites < C3_MAX_COLLINEAR_SPRITES
		   && (hori_index < caster->hori_sprites || vert_index < caster->vert_sprites))
	{
		if (hori_index >= caster->hori_sprites && vert_index < caster->vert_sprites)
		{
			if (caster->vert_hits[vert_index + 1].distance_sqared >= caster->wall_distance)
				vert_index++;
			else
				out[sprites++ + 1] = caster->vert_hits[vert_index++ + 1];
		}
		else if (hori_index < caster->hori_sprites && vert_index >= caster->vert_sprites)
		{
			if (caster->hori_hits[hori_index + 1].distance_sqared >= caster->wall_distance)
				hori_index++;
			else
				out[sprites++ + 1] = caster->hori_hits[hori_index++ + 1];
		}
		else if (caster->hori_hits[hori_index + 1].distance_sqared
				 < caster->vert_hits[vert_index + 1].distance_sqared)
		{
			if (caster->hori_hits[vert_index + 1].distance_sqared >= caster->wall_distance)
				hori_index++;
			else
				out[sprites++ + 1] = caster->hori_hits[hori_index++ + 1];
		}
		else
		{
			if (caster->vert_hits[vert_index + 1].distance_sqared >= caster->wall_distance)
				vert_index++;
			else
				out[sprites++ + 1] = caster->vert_hits[vert_index++ + 1];
		}
	}
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
	sprites = c3_cast_ray_to_sprites(&caster, out);
	return (sprites);
}

void	c3_scan(t_c3_state *stat)
{
	int	x;
	int	half_res;
	int	hit_sprites;

	half_res = stat->renderer.resolution_x / 2;
	x = -half_res;
	while (x < half_res)
	{
		double a =
			x * (stat->renderer.plane_distance * tan(stat->renderer.fov))
			/ half_res;
		double theta = atan(a / stat->renderer.plane_distance);
		stat->renderer.rays[x + half_res].angle = theta;

		double angle = theta + stat->player.direction;
		if (angle >= M_PI * 2)
			angle -= M_PI * 2;
		if (angle < 0)
			angle += M_PI * 2;
		hit_sprites = c3_cast_ray(
			stat, &stat->player.position, angle,
			stat->renderer.rays[x + half_res].hits);
		stat->renderer.rays[x + half_res].hit_sprite_count = hit_sprites;
		x++;
	}
}

void	c3_update(t_c3_state *stat)
{
	if (stat->keystate.left)
	{
		stat->player.direction -= stat->player.rotation_speed;
		while (stat->player.direction < 0)
			stat->player.direction += M_PI * 2;
	}
	else if (stat->keystate.right)
	{
		stat->player.direction += stat->player.rotation_speed;
		while (stat->player.direction >= M_PI * 2)
			stat->player.direction -= M_PI * 2;
	}

	if (stat->keystate.w || stat->keystate.s)
	{
		double delta_x = stat->player.walk_speed * cos(stat->player.direction);
		double delta_y = stat->player.walk_speed * sin(stat->player.direction);
		double new_x = stat->player.position.x + delta_x * (stat->keystate.w ? 1 : -1);
		double new_y = stat->player.position.y + delta_y * (stat->keystate.w ? 1 : -1);
		if (new_x >= 0 && new_x < stat->map.width
			&& new_y >= 0 && new_y < stat->map.height)
		{
			stat->player.position.x = new_x;
			stat->player.position.y = new_y;
		}
	}

	if (stat->keystate.a || stat->keystate.d)
	{
		double delta_x = stat->player.walk_speed * sin(stat->player.direction);
		double delta_y = stat->player.walk_speed * -cos(stat->player.direction);
		double new_x = stat->player.position.x + delta_x * (stat->keystate.a ? 1 : -1);
		double new_y = stat->player.position.y + delta_y * (stat->keystate.a ? 1 : -1);
		if (new_x >= 0 && new_x < stat->map.width
			&& new_y >= 0 && new_y < stat->map.height)
		{
			stat->player.position.x = new_x;
			stat->player.position.y = new_y;
		}
	}

	c3_scan(stat);
}

int		c3_loop_hook(void *param)
{
	t_c3_state	*stat;
	static int	current_frame;

	stat = (t_c3_state*)param;
	if (stat->is_benchmarking && current_frame++ > 1000)
	{
		c3_terminate(stat);
		exit(0);
	}
	c3_update(stat);
	c3_draw(stat);
	return (1);
}
