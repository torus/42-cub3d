/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:23:13 by thisai            #+#    #+#             */
/*   Updated: 2021/02/23 18:52:56 by thisai           ###   ########.fr       */
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

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

#include <mlx.h>

#include "cub3d.h"
#include "cub3d_int.h"
#include "scene.h"

void	c3_renderer_cleanup(t_c3_renderer *rend)
{
	free(rend->rays);
}

void	c3_map_init(t_c3_map *map, t_c3_scene *scene)
{
	int		i;
	char	ch;

	map->map = scene->map;
	map->width = scene->map_width;
	map->height = scene->map_height;
	i = 0;
	while (i < map->width * map->height)
	{
		ch = map->map[i];
		if (ch != ' ' && ch != '0' && ch != '1' && ch != '2' &&
			ch != 'N' && ch != 'E' && ch != 'S' && ch != 'W')
		{
			c3_log("Error\nInvalid map.\n");
			exit(1);
		}
		i++;
	}
}

double	c3_get_direction_from_symbol(char ch)
{
	if (ch == 'N')
		return (-M_PI_2);
	else if (ch == 'E')
		return (0);
	else if (ch == 'S')
		return (M_PI_2);
	return (M_PI);
}

int		c3_player_set_initial_position(t_c3_player *player, t_c3_map *map)
{
	int		i;
	int		init_pos_found;
	char	ch;

	init_pos_found = 0;
	i = 0;
	while (i < map->width * map->height)
	{
		ch = map->map[i];
		if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
		{
			if (init_pos_found)
			{
				c3_log("Error\nMultiple start position contained in the map\n");
				exit(1);
			}
			init_pos_found = 1;
			player->direction = c3_get_direction_from_symbol(ch);
			player->position.x = i % map->width + 0.5;
			player->position.y = i / map->width + 0.5;
		}
		i++;
	}
	return (init_pos_found);
}

void	c3_player_init(t_c3_player *player, t_c3_map *map)
{
	int		init_pos_found;

	init_pos_found = c3_player_set_initial_position(player, map);
	if (!init_pos_found)
	{
		c3_log("Error\nStart position not found in the map\n");
		exit(1);
	}
	player->walk_speed = 0.01;
	player->rotation_speed = 0.01;
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

void	c3_keystate_init(t_c3_keystate *keystat)
{
	keystat->w = 0;
	keystat->a = 0;
	keystat->s = 0;
	keystat->d = 0;
	keystat->left = 0;
	keystat->right = 0;
}

void	c3_check(int64_t val, const char *message)
{
	if (!val)
	{
		c3_log("Error\nCheck failed: %s\n", message);
		exit(1);
	}
}

void	c3_terminate(t_c3_state *stat)
{
	int	tmp;
	int	i;

	tmp = mlx_do_key_autorepeaton(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeaton() returned false.");
	mlx_destroy_window(stat->mlx, stat->window);
	if (stat->img)
		mlx_destroy_image(stat->mlx, stat->img);
	mlx_loop_end(stat->mlx);
	c3_renderer_cleanup(&stat->renderer);
	i = 0;
	while (i < C3_OBJTYPE_NUM)
	{
		if (stat->texture_cache->cache[i].image)
			mlx_destroy_image(stat->mlx, stat->texture_cache->cache[i].image);
		i++;
	}
	mlx_destroy_display(stat->mlx);
	free(stat->mlx);
	c3_scene_cleanup(stat->scene);
}

int		c3_key_press_hook(int key, void *param)
{
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	if (key == XK_W || key == XK_w)
		stat->keystate.w = 1;
	else if (key == XK_A || key == XK_a)
		stat->keystate.a = 1;
	else if (key == XK_S || key == XK_s)
		stat->keystate.s = 1;
	else if (key == XK_D || key == XK_d)
		stat->keystate.d = 1;
	else if (key == XK_Left)
		stat->keystate.left = 1;
	else if (key == XK_Right)
		stat->keystate.right = 1;
	return (1);
}

int		c3_key_release_hook(int key, void *param)
{
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	if (key == XK_Escape)
	{
		c3_terminate(stat);
		exit(0);
	}
	if (key == XK_W || key == XK_w)
		stat->keystate.w = 0;
	else if (key == XK_A || key == XK_a)
		stat->keystate.a = 0;
	else if (key == XK_S || key == XK_s)
		stat->keystate.s = 0;
	else if (key == XK_D || key == XK_d)
		stat->keystate.d = 0;
	else if (key == XK_Left)
		stat->keystate.left = 0;
	else if (key == XK_Right)
		stat->keystate.right = 0;
	else if (key == XK_Tab)
		stat->is_drawing_minimap = !stat->is_drawing_minimap;
	return (1);
}

char	c3_query_map(t_c3_state *stat, int x, int y)
{
	char ch;

	ch = stat->map.map[y * stat->map.width + x];
	if (ch == '1')
		return (1);
	else if (ch == '0')
		return (0);
	else if (ch == '2')
		return (2);
	return (0);
}

double	c3_distance_squared(t_c3_vector *p1, t_c3_vector *p2)
{
	double	dx;
	double	dy;

	dx = p1->x - p2->x;
	dy = p1->y - p2->y;
	return (dx * dx + dy * dy);
}

void		c3_texture_cache_load(
	t_c3_state *stat, t_c3_object_type type)
{
	int				width;
	int				height;
	void			*image;
	t_c3_texture	*tex;
	char			*path;

	tex = &stat->texture_cache->cache[type];
	path = stat->texture_cache->path[type];
	image = mlx_xpm_file_to_image(stat->mlx, path, &width, &height);
	if (!image)
	{
		c3_log("Error\ntexture not found.\n");
		c3_terminate(stat);
		exit(1);
	}
	tex->image = image;
	tex->width = width;
	tex->height = height;
	stat->texture_cache->cache[type].data = mlx_get_data_addr(
		image, &tex->bits_per_pixel, &tex->size_line, &tex->endian);
}

uint32_t	c3_sample_texture(
	t_c3_state *stat, t_c3_object_type type, double u, double v)
{
	uint32_t			texcol;
	t_c3_texture_cache	*cache;
	t_c3_texture		*tex;
	int					index;

	cache = stat->texture_cache;
	tex = &cache->cache[type];
	if (!tex->image)
		c3_texture_cache_load(stat, type);
	index = (int)(v * tex->height) * tex->size_line
		+ u * tex->width * tex->bits_per_pixel / 8;
	texcol = ((uint32_t*)tex->data)[index / 4];
	return (texcol);
}

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

int		c3_render_test_sprite(
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

void	c3_render_fill_pixel(t_c3_state *stat, int x, int y, uint32_t col)
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

uint32_t	c3_render_get_wall_pixel(
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

void	c3_render_scene(t_c3_state *stat)
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

int		c3_focusin_hook(void *param)
{
	int			tmp;
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	tmp = mlx_do_key_autorepeatoff(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeatoff() returned false.");
	return (1);
}

int		c3_focusout_hook(void *param)
{
	int			tmp;
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	tmp = mlx_do_key_autorepeaton(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeatoff() returned false.");
	return (1);
}

int		c3_client_hook(void *param)
{
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	c3_terminate(stat);
	exit(0);
}

void	c3_check_map_closed_iter(t_c3_state *stat, int x, int y)
{
	char	ch;

	if (x >= 0 && x < stat->map.width && y >= 0 && y < stat->map.height)
	{
		ch = stat->map.map[y * stat->map.width + x];
		if (ch != '1' && ch != 'x' && ch != 'o')
		{
			if (ch == '2')
				stat->map.map[y * stat->map.width + x] = 'o';
			else
				stat->map.map[y * stat->map.width + x] = 'x';
			c3_check_map_closed_iter(stat, x, y - 1);
			c3_check_map_closed_iter(stat, x + 1, y);
			c3_check_map_closed_iter(stat, x, y + 1);
			c3_check_map_closed_iter(stat, x - 1, y);
		}
		return ;
	}
	c3_log("Error\nThe map is not closed.\n");
	exit(1);
}

void	c3_check_map_closed(t_c3_state *stat, int x, int y)
{
	char	ch;

	c3_check_map_closed_iter(stat, x, y);
	y = 0;
	while (y < stat->map.height)
	{
		x = 0;
		while (x < stat->map.width)
		{
			ch = stat->map.map[y * stat->map.width + x];
			if (ch == 'x')
				stat->map.map[y * stat->map.width + x] = '0';
			else if (ch == 'o')
				stat->map.map[y * stat->map.width + x] = '2';
			x++;
		}
		y++;
	}
}

void	c3_init_hooks(t_c3_state *stat)
{
	C3_CHECK(
		mlx_key_hook(stat->window, c3_key_release_hook, stat),
		"mlx_key_hook() returned false.");
	C3_CHECK(
		mlx_hook(stat->window, KeyPress, KeyPressMask, c3_key_press_hook, stat),
		"mlx_hook() returned false.");
	C3_CHECK(
		mlx_hook(stat->window, FocusIn, FocusChangeMask, c3_focusin_hook, stat),
		"mlx_hook() returned false.");
	C3_CHECK(
		mlx_hook(
			stat->window, FocusOut, FocusChangeMask, c3_focusout_hook, stat),
		"mlx_hook() returned false.");
	C3_CHECK(
		mlx_hook(
			stat->window, ClientMessage, NoEventMask, c3_client_hook, stat),
		"mlx_hook() returned false.");
	C3_CHECK(
		mlx_loop_hook(stat->mlx, c3_loop_hook, stat),
		"mlx_loop_hook() returned false.");
	C3_CHECK(
		mlx_do_key_autorepeatoff(stat->mlx),
		"mlx_do_key_autorepeatoff() returned false.");
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
}

void	c3_init_render_target(t_c3_state *stat)
{
	stat->img = mlx_new_image(
		stat->mlx, stat->screen_width, stat->screen_height);
	C3_CHECK(stat->img, "mlx_new_image() returned NULL.");
	stat->imgdata.data = mlx_get_data_addr(
		stat->img, &stat->imgdata.bits_per_pixel,
		&stat->imgdata.size_line, &stat->imgdata.endian);
}

void	c3_init(t_c3_state *stat, t_c3_texture_cache *tex, t_c3_scene *scene)
{
	stat->scene = scene;
	c3_map_init(&stat->map, scene);
	c3_keystate_init(&stat->keystate);
	c3_player_init(&stat->player, &stat->map);
	c3_check_map_closed(stat, stat->player.position.x, stat->player.position.y);
	stat->mlx = mlx_init();
	C3_CHECK(stat->mlx, "mlx is NULL.");
	c3_init_set_screen_size(stat, scene);
	c3_renderer_init(
		&stat->renderer, scene, stat->map.width * 8, stat->map.height * 8);
	stat->window = mlx_new_window(
		stat->mlx, stat->screen_width, stat->screen_height, "Cub3D!");
	C3_CHECK(stat->window, "window is NULL.");
	c3_init_render_target(stat);
	c3_texture_cache_init(tex, scene);
	stat->texture_cache = tex;
	c3_init_hooks(stat);
	stat->is_drawing_minimap = 0;
	stat->is_benchmarking = 0;
	stat->is_showing_screen = 1;
}

int		c3_read_scene(t_c3_scene *scene, const char *path)
{
	t_c3_scene_parser	buf;
	t_c3_file			file;

	buf.container.file = &file;
	if (!c3_scene_parser_init_with_file(&buf, path))
	{
		c3_log("Error\n%s\n", buf.error);
		return (0);
	}
	c3_scene_init(scene);
	if (c3_scene_parse(scene, &buf) != C3_PARSE_SUCCESS)
	{
		c3_log("Error\n%s\n", buf.error);
		return (0);
	}
	return (1);
}

int		main(int argc, char **argv)
{
	t_c3_state			stat;
	t_c3_texture_cache	tex;
	t_c3_scene			scene;

	if (argc < 2 || argc > 3)
	{
		c3_log("Error\nusage: cub3d scene.cub [--save]\n");
		return (1);
	}
	c3_read_scene(&scene, argv[1]);
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
