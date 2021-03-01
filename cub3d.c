/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:23:13 by thisai            #+#    #+#             */
/*   Updated: 2021/02/26 11:52:09 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <X11/XKBlib.h>
#include <mlx.h>

#include "cub3d.h"
#include "cub3d_int.h"
#include "scene.h"

void	c3_keystate_init(t_c3_keystate *keystat)
{
	keystat->w = 0;
	keystat->a = 0;
	keystat->s = 0;
	keystat->d = 0;
	keystat->left = 0;
	keystat->right = 0;
}

void	c3_texture_cache_init(t_c3_texture_cache *cache, t_c3_scene *scene)
{
	int	i;

	cache->path = scene->tex_path;
	i = 0;
	while (i < C3_OBJTYPE_NUM)
	{
		cache->cache[i].image = NULL;
		i++;
	}
}

void	c3_init_hooks(t_c3_state *stat)
{
	c3_check(!!
		mlx_key_hook(stat->window, c3_key_release_hook, stat),
		"mlx_key_hook() returned false.");
	c3_check(!!
		mlx_hook(stat->window, KeyPress, KeyPressMask, c3_key_press_hook, stat),
		"mlx_hook() returned false.");
	c3_check(!!
		mlx_hook(stat->window, FocusIn, FocusChangeMask, c3_focusin_hook, stat),
		"mlx_hook() returned false.");
	c3_check(!!
		mlx_hook(
			stat->window, FocusOut, FocusChangeMask, c3_focusout_hook, stat),
		"mlx_hook() returned false.");
	c3_check(!!
		mlx_hook(
			stat->window, ClientMessage, NoEventMask, c3_client_hook, stat),
		"mlx_hook() returned false.");
	c3_check(!!
		mlx_loop_hook(stat->mlx, c3_loop_hook, stat),
		"mlx_loop_hook() returned false.");
	c3_check(!!
		mlx_do_key_autorepeatoff(stat->mlx),
		"mlx_do_key_autorepeatoff() returned false.");
}

void	c3_init(t_c3_state *stat, t_c3_texture_cache *tex, t_c3_scene *scene)
{
	stat->mlx = NULL;
	stat->window = NULL;
	stat->scene = scene;
	stat->renderer.rays = NULL;
	c3_map_init(&stat->map, scene);
	c3_keystate_init(&stat->keystate);
	c3_player_init(stat, &stat->player, &stat->map);
	c3_check_map_closed(stat, stat->player.position.x, stat->player.position.y);
	stat->mlx = mlx_init();
	c3_check(!!stat->mlx, "mlx is NULL.");
	c3_init_set_screen_size(stat, scene);
	c3_renderer_init(
		&stat->renderer, scene, stat->map.width * 8, stat->map.height * 8);
	stat->window = mlx_new_window(
		stat->mlx, stat->screen_width, stat->screen_height, "Cub3D!");
	c3_check(!!stat->window, "window is NULL.");
	c3_init_render_target(stat);
	c3_texture_cache_init(tex, scene);
	stat->texture_cache = tex;
	c3_init_hooks(stat);
	stat->is_drawing_minimap = 0;
	stat->is_benchmarking = 0;
	stat->is_showing_screen = 1;
}

int		main(int argc, char **argv)
{
	t_c3_state			stat;
	t_c3_texture_cache	tex;
	t_c3_scene			scene;

	if (argc < 2 || argc > 3)
	{
		C3_LOG("Error\nusage: cub3d scene.cub [--save]\n");
		return (1);
	}
	if (!c3_read_scene(&scene, argv[1]))
	{
		c3_scene_cleanup(&scene);
		return (1);
	}
	c3_init(&stat, &tex, &scene);
	if (argc == 3)
	{
		stat.is_showing_screen = 0;
		c3_bmp_generate(&stat);
		c3_terminate(&stat);
	}
	else
		mlx_loop(stat.mlx);
	return (0);
}
