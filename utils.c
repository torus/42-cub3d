/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 18:07:22 by thisai            #+#    #+#             */
/*   Updated: 2021/03/01 14:20:44 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

double	c3_dot(t_c3_vector *origin, t_c3_vector *a, t_c3_vector *b)
{
	return (
		(a->x - origin->x) * (b->x - origin->x)
		+ (a->y - origin->y) * (b->y - origin->y));
}

double	c3_distance_squared(t_c3_vector *p1, t_c3_vector *p2)
{
	double	dx;
	double	dy;

	dx = p1->x - p2->x;
	dy = p1->y - p2->y;
	return (dx * dx + dy * dy);
}

void	c3_check(int64_t val, const char *message)
{
	if (!val)
	{
		C3_LOG("Error\nCheck failed: %s\n", message);
		exit(1);
	}
}

void	c3_terminate(t_c3_state *stat)
{
	int	tmp;
	int	i;

	if (stat->mlx)
	{
		tmp = mlx_do_key_autorepeaton(stat->mlx);
		c3_check(!!tmp, "mlx_do_key_autorepeaton() returned false.");
		if (stat->window)
			mlx_destroy_window(stat->mlx, stat->window);
		if (stat->img)
			mlx_destroy_image(stat->mlx, stat->img);
		mlx_loop_end(stat->mlx);
		i = 0;
		while (stat->texture_cache && i < C3_OBJTYPE_NUM)
		{
			if (stat->texture_cache->cache[i].image)
				mlx_destroy_image(
					stat->mlx, stat->texture_cache->cache[i].image);
			i++;
		}
		mlx_destroy_display(stat->mlx);
		free(stat->mlx);
	}
	c3_renderer_cleanup(&stat->renderer);
	c3_scene_cleanup(stat->scene);
}
