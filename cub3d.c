/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:23:13 by thisai            #+#    #+#             */
/*   Updated: 2021/02/15 12:53:01 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

#include <mlx.h>

#include "cub3d.h"
#include "scene.h"


const int	c3_texture_size = 32;

const char	c3_player_bitmap[] = {
	0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};
const int	c3_player_bitmap_height = 8;
const int	c3_player_bitmap_width = 8;


void	c3_renderer_cleanup(t_c3_renderer *rend)
{
	free(rend->rays);
}

void	c3_map_init(t_c3_map *map, t_c3_scene *scene)
{
	map->map = scene->map;
	map->width = scene->map_width;
	map->height = scene->map_height;
}

void	c3_player_init(t_c3_player *player, t_c3_map *map)
{
	int		i;
	int		init_pos_found;
	char	ch;

	init_pos_found = 0;
	i = 0;
	while(i < map->width * map->height)
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
			if (ch == 'N')
				player->direction = -M_PI_2;
			else if (ch == 'E')
				player->direction = 0;
			else if (ch == 'S')
				player->direction = M_PI_2;
			else
				player->direction = M_PI;
			player->position.x = i % map->width;
			player->position.y = i / map->width;
		}
		i++;
	}
	if (!init_pos_found)
	{
		c3_log("Error\nStart position not found in the map\n");
		exit(1);
	}
	player->walk_speed = 0.01;
	player->rotation_speed = 0.01;
}

void	c3_renderer_init(t_c3_renderer *rend, int minimap_width, int minimap_height)
{
	rend->plane_distance = 1.;
	rend->fov = M_PI / 6.;
	rend->resolution_x = 320;
	rend->resolution_y = 180;
	rend->rays = malloc(sizeof(t_c3_ray) * rend->resolution_x);
	rend->minimap_width = minimap_width;
	rend->minimap_height = minimap_height;
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

	tmp = mlx_do_key_autorepeaton(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeaton() returned false.");

	mlx_destroy_window(stat->mlx, stat->window);
	if (stat->img)
		mlx_destroy_image(stat->mlx, stat->img);

	mlx_loop_end(stat->mlx);

	c3_renderer_cleanup(&stat->renderer);

	int	i;
	i = 0;
	while (i < C3_OBJTYPE_NUM)
	{
		if (stat->texture_cache->cache[i].image)
			mlx_destroy_image(stat->mlx, stat->texture_cache->cache[i].image);
		i++;
	}

	mlx_destroy_display(stat->mlx);
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
	return (1);
}

void	c3_draw_player_on_map(t_c3_state *stat)
{
	int x = stat->player.position.x * stat->renderer.minimap_width / stat->map.width;
	int y = stat->player.position.y * stat->renderer.minimap_height / stat->map.height;
	double angle = stat->player.direction;
	int max_index = stat->renderer.resolution_x * stat->imgdata.size_line;

	for (int i = -8; i < 9; i++)
	{
		for (int j = -8; j < 9; j++)
		{
			int xx = j * cos(-angle) - i * sin(-angle);
			int yy = j * sin(-angle) + i * cos(-angle) + 3;
			if (xx >= 0 && xx < c3_player_bitmap_width
				&& yy >= 0 && yy < c3_player_bitmap_height
				&& c3_player_bitmap[yy * c3_player_bitmap_width + xx])
			{
				int index =
					(i + y) * stat->imgdata.size_line +
					(j + x) * stat->imgdata.bits_per_pixel / 8;
				if (index >= 0 && index < max_index)
				{
					unsigned int col = mlx_get_color_value(
						stat->mlx, (0 << 24) + (0 << 16) + (255 << 8));
					stat->imgdata.data[index + 0] = (col >> 24) & 0xff;
					stat->imgdata.data[index + 1] = (col >> 16) & 0xff;
					stat->imgdata.data[index + 2] = (col >> 8) & 0xff;
					stat->imgdata.data[index + 3] = col & 0xff;
				}
			}
		}
	}
}

char	c3_query_map(t_c3_state *stat, int x, int y)
{
	char ch = stat->map.map[y * stat->map.width + x];

	switch (ch)
	{
	case '1':
		return 1;
	case '0':
		return 0;
	case '2':
		return 2;
	default:
		return 0;
	}
	/* return stat->map.map[y * stat->map.width + x]; */
}

void	c3_draw_map(t_c3_state *stat)
{
	for (int i = 0; i < stat->screen_height && i < stat->renderer.minimap_height; i++)
	{
		int y = stat->map.height * i / stat->renderer.minimap_height;
		for (int j = 0; j < stat->screen_width && j < stat->renderer.minimap_width; j++)
		{
			int x = stat->map.width * j / stat->renderer.minimap_width;
			/* int cell = stat->map.map[y * stat->map.width + x]; */
			int cell = c3_query_map(stat, x, y);

			int r = 127 * (1 - cell) + 128;
			int g = 127 * (1 - cell) + 128;
			int b = 127 * (1 - cell) + 128;

			unsigned int col = mlx_get_color_value(
				stat->mlx, (r << 24) + (g << 16) + (b << 8));
			int index =
				i * stat->imgdata.size_line +
				j * stat->imgdata.bits_per_pixel / 8;
			stat->imgdata.data[index + 0] = (col >> 24) & 0xff;
			stat->imgdata.data[index + 1] = (col >> 16) & 0xff;
			stat->imgdata.data[index + 2] = (col >> 8) & 0xff;
			stat->imgdata.data[index + 3] = col & 0xff;
		}
	}
	c3_draw_player_on_map(stat);
}

double	c3_distance_squared(t_c3_vector *p1, t_c3_vector *p2)
{
	double	dx;
	double	dy;

	dx = p1->x - p2->x;
	dy = p1->y - p2->y;
	return (dx * dx + dy * dy);
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

int		c3_check_sprite(t_c3_state *stat, t_c3_vector *center, t_c3_vector *pos, t_c3_hit_result *result)
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
		if (theta >= 0 && theta < M_PI)
		{
			hit.y = floor(pos->y) + index;
			hit.x = pos->x + (hit.y - pos->y) / tan(theta);
			facing_north = 0;
			hit_cell.x = floor(hit.x) + 0.5;
			hit_cell.y = floor(hit.y) + 0.5;
		}
		else
		{
			hit.y = floor(pos->y) - index + 1;
			hit.x = pos->x + (hit.y - pos->y) / tan(theta);
			facing_north = 1;
			hit_cell.x = floor(hit.x) + 0.5;
			hit_cell.y = floor(hit.y) - 1 + 0.5;
		}

		if (c3_check_wall(stat, &hit_cell))
			break ;
		if (hit_sprites < C3_MAX_COLLINEAR_SPRITES
			&& c3_check_sprite(stat, &hit_cell, pos, &result[hit_sprites + 1]))
		{

			double dot = c3_dot(pos, &hit_cell, &hit);
			t_c3_vector	ad = {hit.x - pos->x, hit.y - pos->y};
			double	c = result[hit_sprites + 1].distance_sqared / dot;
			hit.x = pos->x + ad.x * c;
			hit.y = pos->y + ad.y * c;

			double	offset = sqrt(c3_distance_squared(&hit, &hit_cell));
			if (offset <= 0.5)
			{
				t_c3_vector	ab = {hit_cell.x - pos->x, hit_cell.y - pos->y};
				double	cross = ab.x * ad.y - ad.x * ab.y;

				if (cross > 0)
					offset = - offset;

				result[hit_sprites + 1].position = hit;
				result[hit_sprites + 1].offset = offset;
				hit_sprites ++;
			}
		}

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
	int			i;
	int			hit_sprites;

	hit_sprites = 0;
	i = 1;
	while (1)
	{
		if (theta < M_PI_2 || theta >= 3 * M_PI_2)
		{
			hit.x = floor(pos->x) + i;
			hit.y = pos->y + (hit.x - pos->x) * tan(theta);
			facing_east = 1;
			hit_cell.x = floor(hit.x) + 0.5;
			hit_cell.y = floor(hit.y) + 0.5;
		}
		else
		{
			hit.x = floor(pos->x) - i + 1;
			hit.y = pos->y + (hit.x - pos->x) * tan(theta);
			facing_east = 0;
			hit_cell.x = floor(hit.x) - 1 + 0.5;
			hit_cell.y = floor(hit.y) + 0.5;
		}

		if (c3_check_wall(stat, &hit_cell))
			break ;
		if (hit_sprites < C3_MAX_COLLINEAR_SPRITES
			&& c3_check_sprite(stat, &hit_cell, pos, &result[hit_sprites + 1]))
		{
			double dot = c3_dot(pos, &hit_cell, &hit);
			t_c3_vector	ad = {hit.x - pos->x, hit.y - pos->y};
			double	c = result[hit_sprites + 1].distance_sqared / dot;
			hit.x = pos->x + ad.x * c;
			hit.y = pos->y + ad.y * c;

			double	offset = sqrt(c3_distance_squared(&hit, &hit_cell));
			if (offset <= 0.5)
			{
				t_c3_vector	ab = {hit_cell.x - pos->x, hit_cell.y - pos->y};
				double	cross = ab.x * ad.y - ad.x * ab.y;

				if (cross > 0)
					offset = - offset;

				result[hit_sprites + 1].position = hit;
				result[hit_sprites + 1].offset = offset;
				hit_sprites ++;
			}
		}

		i++;
	}
	result->position = hit;
	result->distance_sqared = c3_distance_squared(pos, &hit);
	result->type = facing_east ? C3_OBJTYPE_WALL_E : C3_OBJTYPE_WALL_W;
	return (hit_sprites);
}

int		c3_cast_ray(
	t_c3_state *stat, t_c3_vector *pos, double theta, t_c3_hit_result *out)
{
	t_c3_hit_result	hori_hits[1 + C3_MAX_COLLINEAR_SPRITES];
	t_c3_hit_result	vert_hits[1 + C3_MAX_COLLINEAR_SPRITES];
	int				hori_sprites;
	int				vert_sprites;
	int				hori_index;
	int				vert_index;
	int				sprites;
	double			wall_distance;

	hori_sprites = tan(theta) != 0.0 ? c3_get_horizontal_hit(stat, pos, theta, hori_hits) : 0;
	vert_sprites = c3_get_vertical_hit(stat, pos, theta, vert_hits);

	if (tan(theta) != 0.0 &&
		hori_hits[0].distance_sqared < vert_hits[0].distance_sqared)
	{
		*out = hori_hits[0];
		wall_distance = hori_hits[0].distance_sqared;
	}
	else
	{
		*out = vert_hits[0];
		wall_distance = vert_hits[0].distance_sqared;
	}

	hori_index = 0;
	vert_index = 0;
	sprites = 0;
	while (sprites < C3_MAX_COLLINEAR_SPRITES
		   && (hori_index < hori_sprites || vert_index < vert_sprites))
	{
		if (hori_index >= hori_sprites && vert_index < vert_sprites)
		{
			if (vert_hits[vert_index + 1].distance_sqared >= wall_distance)
				vert_index++;
			else
				out[sprites++ + 1] = vert_hits[vert_index++ + 1];
		}
		else if (hori_index < hori_sprites && vert_index >= vert_sprites)
		{
			if (hori_hits[hori_index + 1].distance_sqared >= wall_distance)
				hori_index++;
			else
				out[sprites++ + 1] = hori_hits[hori_index++ + 1];
		}
		else if (hori_hits[hori_index + 1].distance_sqared
				 < vert_hits[vert_index + 1].distance_sqared)
		{
			if (hori_hits[vert_index + 1].distance_sqared >= wall_distance)
				hori_index++;
			else
				out[sprites++ + 1] = hori_hits[hori_index++ + 1];
		}
		else
		{
			if (vert_hits[vert_index + 1].distance_sqared >= wall_distance)
				vert_index++;
			else
				out[sprites++ + 1] = vert_hits[vert_index++ + 1];
		}
	}
	return (sprites);
}

void	c3_draw_rays_on_map(t_c3_state *stat)
{
	double		world_x;
	double		world_y;
	double		screen_x;
	double		screen_y;
	int			x;
	int			i;

	x = 0;
	while (x < stat->renderer.resolution_x)
	{
		world_x = stat->renderer.rays[x].hits[0].position.x;
		world_y = stat->renderer.rays[x].hits[0].position.y;
		screen_x = world_x * stat->renderer.minimap_width / stat->map.width;
		screen_y = world_y * stat->renderer.minimap_height / stat->map.height;

		int r = 255 * x / stat->renderer.resolution_x;
		int col = (r << 16) + ((255 - r) << 0);
		mlx_string_put(
			stat->mlx, stat->window, screen_x, screen_y,
			mlx_get_color_value(stat->mlx, col), "*");

		i = 0;
		while (i < stat->renderer.rays[x].hit_sprite_count)
		{
			world_x = stat->renderer.rays[x].hits[i + 1].position.x;
			world_y = stat->renderer.rays[x].hits[i + 1].position.y;
			screen_x = world_x * stat->renderer.minimap_width / stat->map.width;
			screen_y = world_y * stat->renderer.minimap_height / stat->map.height;

			int r = 255 * x / stat->renderer.resolution_x;
			int col = (r << 16) + ((255 - r) << 0);
			mlx_string_put(
				stat->mlx, stat->window, screen_x, screen_y,
				mlx_get_color_value(stat->mlx, col), "x");
			i++;
		}

		x++;
	}
}

void		c3_texture_cache_load(
	t_c3_state *stat, t_c3_object_type type, char *path)
{
	int				width;
	int				height;
	void			*image;
	t_c3_texture	*tex;

	tex = &stat->texture_cache->cache[type];

	image = mlx_xpm_file_to_image(stat->mlx, path, &width, &height);
	tex->image = image;
	stat->texture_cache->cache[type].data = mlx_get_data_addr(
		image, &tex->bits_per_pixel, &tex->size_line, &tex->endian);
}

uint32_t	c3_sample_texture(
	t_c3_state *stat, t_c3_object_type type, int u, int v)
{
	uint32_t			texcol;
	t_c3_texture_cache	*cache;
	t_c3_texture		*tex;

	char	*path[] = {
		"wall-1.xpm",
		"wall-2.xpm",
		"wall-3.xpm",
		"wall-4.xpm",
		"sprite-1.xpm",
	};

	cache = stat->texture_cache;
	tex = &cache->cache[type];
	if (!tex->image)
		c3_texture_cache_load(stat, type, path[type]);

	int	index = v * tex->size_line + u * tex->bits_per_pixel / 8;

	texcol = ((uint32_t*)tex->data)[index / 4];

	return (texcol);
}

unsigned int	c3_wall_texel(
	t_c3_state *stat, t_c3_ray *ray, int wall_height, int y)
{
	unsigned int	col;
	int				v;

	v = c3_texture_size *
		(y - (stat->renderer.resolution_y - wall_height) / 2) /
		wall_height;

	if (ray->hits[0].type == C3_OBJTYPE_WALL_N)
		col = c3_sample_texture(
			stat,
			C3_OBJTYPE_WALL_N,
			(int)(ray->hits[0].position.x * c3_texture_size) % c3_texture_size,
			v);

	else if (ray->hits[0].type == C3_OBJTYPE_WALL_E)
		col = c3_sample_texture(
			stat,
			C3_OBJTYPE_WALL_E,
			(int)(ray->hits[0].position.y * c3_texture_size) % c3_texture_size,
			v);

	else if (ray->hits[0].type == C3_OBJTYPE_WALL_S)
		col = c3_sample_texture(
			stat,
			C3_OBJTYPE_WALL_S,
			c3_texture_size - (int)(ray->hits[0].position.x * c3_texture_size) % c3_texture_size,
			v);

	else //(ray->hit.type == C3_OBJTYPE_WALL_W)
		col = c3_sample_texture(
			stat,
			C3_OBJTYPE_WALL_W,
			c3_texture_size - (int)(ray->hits[0].position.y * c3_texture_size) % c3_texture_size,
			v);

	return (col);
}

void	c3_draw_walls(t_c3_state *stat)
{
	int				x;
	int				y;
	t_c3_ray		*ray;
	unsigned int	col;
	int				screen_y;

	uint32_t	ceiling_r = 0x30;
	uint32_t	ceiling_g = 0x88;
	uint32_t	ceiling_b = 0xff;

	uint32_t	floor_r = 0x50;
	uint32_t	floor_g = 0xc0;
	uint32_t	floor_b = 0x50;

	x = 0;
	while (x < stat->renderer.resolution_x)
	{
		ray = &stat->renderer.rays[x];

		int wall_height =
			stat->renderer.resolution_y
			/ (sqrt(ray->hits[0].distance_sqared) * cos(ray->angle));

		y = 0;
		while (y < stat->renderer.resolution_y)
		{
			int	sprites;
			int	i;
			int	found_sprite;
			double	distance;

			found_sprite = 0;
			sprites = ray->hit_sprite_count;
			i = 0;
			while (i < sprites)
			{
				distance = sqrt(ray->hits[i + 1].distance_sqared);

				int sprite_height =
					stat->renderer.resolution_y
					/ distance;

				if (y < (stat->renderer.resolution_y - sprite_height) / 2)
				{
					col = mlx_get_color_value(
						stat->mlx, (ceiling_r << 16) + (ceiling_g << 8) + ceiling_b);
					break ;
				}
				else if (y >= (stat->renderer.resolution_y + sprite_height) / 2)
				{
					col = mlx_get_color_value(
						stat->mlx, (floor_r << 16) + (floor_g << 8) + floor_b);
					break ;
				}


				int v = c3_texture_size *
					(y - (stat->renderer.resolution_y - sprite_height) / 2) /
					sprite_height;

				double	offset = ray->hits[i + 1].offset;
				if ((int)(distance * 5) % 2)
					offset = -offset;

				int	u = floor(c3_texture_size * (offset + 0.5));
				if (u >= 0 && u < c3_texture_size)
				{
					col = c3_sample_texture(
						stat,
						C3_OBJTYPE_SPRITE,
						u,
						v);
					if (col != C3_TRANSPARENT_COLOR)
					{
						found_sprite = 1;
						break ;
					}
				}

				i++;
			}
			if (!found_sprite)
			{
				if (y < (stat->renderer.resolution_y - wall_height) / 2)
					col = mlx_get_color_value(
						stat->mlx, (ceiling_r << 16) + (ceiling_g << 8) + ceiling_b);
				else if (y >= (stat->renderer.resolution_y + wall_height) / 2)
					col = mlx_get_color_value(
						stat->mlx, (floor_r << 16) + (floor_g << 8) + floor_b);
				else
					col = c3_wall_texel(stat, ray, wall_height, y);
			}

			screen_y = y * stat->screen_height / stat->renderer.resolution_y;
			while (screen_y < (y + 1) * stat->screen_height / stat->renderer.resolution_y)
			{

				for (int screen_x = stat->screen_width * x / stat->renderer.resolution_x;
					 screen_x < stat->screen_width * (x + 1) / stat->renderer.resolution_x;
					 screen_x++)
				{
					int index =
						screen_y * stat->imgdata.size_line +
						screen_x * stat->imgdata.bits_per_pixel / 8;

					((uint32_t*)stat->imgdata.data)[index / 4] = col;
				}
				screen_y++;
			}
			y++;
		}

		x++;
	}
}

void	c3_render_scene(t_c3_state *stat)
{
	c3_draw_walls(stat);
}

void	c3_draw(t_c3_state *stat)
{
	c3_render_scene(stat);
	c3_draw_map(stat);

	mlx_put_image_to_window(stat->mlx, stat->window, stat->img, 0, 0);

	c3_draw_rays_on_map(stat);

	mlx_string_put(
		stat->mlx, stat->window, 10, 10, mlx_get_color_value(stat->mlx, 0xffffff), "CUB3D");
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
	/* static int	current_frame; */

	stat = (t_c3_state*)param;

	/* if (current_frame++ > 1000) */
	/* { */
	/* 	c3_terminate(stat); */
	/* 	exit(0); */
	/* } */

	c3_update(stat);
	c3_draw(stat);
	return (1);
}

void	c3_texture_cache_init(t_c3_texture_cache *cache)
{
	int	i;

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

int		c3_init(t_c3_state *stat, t_c3_texture_cache *tex, t_c3_scene *scene)
{
	int			tmp;

	c3_map_init(&stat->map, scene);

	stat->screen_width = scene->resolution.x;
	stat->screen_height = scene->resolution.y;
	c3_keystate_init(&stat->keystate);
	c3_player_init(&stat->player, &stat->map);
	c3_renderer_init(&stat->renderer, stat->map.width * 8, stat->map.height * 8);

	stat->mlx = mlx_init();
	C3_CHECK(stat->mlx, "mlx is NULL.");
	stat->window = mlx_new_window(
		stat->mlx, stat->screen_width, stat->screen_height, "Cub3D!");
	C3_CHECK(stat->window, "window is NULL.");

	tmp = mlx_key_hook(stat->window, c3_key_release_hook, stat);
	C3_CHECK(tmp, "mlx_key_hook() returned false.");

	tmp = mlx_hook(
		stat->window, KeyPress,
		KeyPressMask,
		c3_key_press_hook, stat);
	C3_CHECK(tmp, "mlx_hook() returned false.");

	tmp = mlx_hook(
		stat->window, FocusIn,
		FocusChangeMask,
		c3_focusin_hook, stat);
	C3_CHECK(tmp, "mlx_hook() returned false.");

	tmp = mlx_hook(
		stat->window, FocusOut,
		FocusChangeMask,
		c3_focusout_hook, stat);
	C3_CHECK(tmp, "mlx_hook() returned false.");

	tmp = mlx_hook(
		stat->window, ClientMessage,
		NoEventMask,
		c3_client_hook, stat);
	C3_CHECK(tmp, "mlx_hook() returned false.");

	stat->img = mlx_new_image(
		stat->mlx, stat->screen_width, stat->screen_height);
	C3_CHECK(stat->img, "mlx_new_image() returned NULL.");
	stat->imgdata.data = mlx_get_data_addr(
		stat->img,
		&stat->imgdata.bits_per_pixel,
		&stat->imgdata.size_line,
		&stat->imgdata.endian);

	tmp = mlx_loop_hook(stat->mlx, c3_loop_hook, stat);
	C3_CHECK(tmp, "mlx_loop_hook() returned false.");

	c3_texture_cache_init(tex);
	stat->texture_cache = tex;

	tmp = mlx_do_key_autorepeatoff(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeatoff() returned false.");

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

	t_c3_scene_parser	buf;
	int					result;
	t_c3_file			file;

	buf.container.file = &file;
	result = c3_scene_parser_init_with_file(&buf, argv[1]);
	if (!result)
	{
		c3_log("Error\n%s\n", buf.error);
		return (1);
	}

	c3_scene_init(&scene);
	if (c3_scene_parse(&scene, &buf) != C3_PARSE_SUCCESS)
	{
		c3_log("Error\n%s\n", buf.error);
		return (1);
	}

	c3_init(&stat, &tex, &scene);

	if (argc == 3)
	{

		return (0);
	}

	mlx_loop(stat.mlx);
	return (0);
}
