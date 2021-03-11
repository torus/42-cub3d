/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 17:43:15 by thisai            #+#    #+#             */
/*   Updated: 2021/02/25 17:43:15 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

void	c3_renderer_cleanup(t_c3_renderer *rend)
{
	free(rend->rays);
}

void	c3_renderer_init(
	t_c3_renderer *rend, t_c3_scene *scene,
	int minimap_width, int minimap_height)
{
	rend->plane_distance = 1.;
	rend->fov = M_PI / 6.;
	rend->resolution_x = 320;
	rend->resolution_y = 180;
	rend->rays = malloc(sizeof(t_c3_ray) * rend->resolution_x);
	rend->minimap_width = minimap_width;
	rend->minimap_height = minimap_height;
	rend->ceiling_color = scene->color_ceiling;
	rend->floor_color = scene->color_floor;
}

void	c3_init_render_target(t_c3_state *stat)
{
	stat->img = mlx_new_image(
		stat->mlx, stat->screen_width, stat->screen_height);
	c3_check(!!stat->img, "mlx_new_image() returned NULL.");
	stat->imgdata.data = mlx_get_data_addr(
		stat->img, &stat->imgdata.bits_per_pixel,
		&stat->imgdata.size_line, &stat->imgdata.endian);
}

void	c3_init_set_screen_size(t_c3_state *stat, t_c3_scene *scene)
{
	int		width;
	int		height;

	mlx_get_screen_size(stat->mlx, &width, &height);
	stat->screen_width = width > scene->resolution.x
		? scene->resolution.x : width;
	stat->screen_height = height > scene->resolution.y
		? scene->resolution.y : height;
	c3_check(
		stat->screen_width > 0 && stat->screen_height > 0,
		"Resolution must be positive.");
}

void	c3_start_rendering_loop(t_c3_state *stat)
{
	stat->window = mlx_new_window(
		stat->mlx, stat->screen_width, stat->screen_height, "Cub3D!");
	c3_check(!!stat->window, "window is NULL.");
	c3_init_hooks(stat);
	mlx_loop(stat->mlx);
}
