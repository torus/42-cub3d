/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:23:13 by thisai            #+#    #+#             */
/*   Updated: 2021/01/17 19:25:57 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

#include <mlx.h>

#define C3_CHECK(val, mesg) c3_check((int64_t)val, mesg)


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

typedef struct	s_c3_imgdata
{
	char	*data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}		t_c3_imgdata;

typedef struct	s_c3_keystate
{
	char	w;
	char	a;
	char	s;
	char	d;
	char	left;
	char	right;
}		t_c3_keystate;

typedef struct	s_c3_player
{
	double	x;
	double	y;
	double	direction;
	double	walk_speed;
	double	rotation_speed;
}		t_c3_player;

typedef struct	s_c3_coord
{
	double	x;
	double	y;
}		t_c3_coord;

typedef enum	e_c3_object_type
{
	C3_OBJTYPE_WALL_N,
	C3_OBJTYPE_WALL_E,
	C3_OBJTYPE_WALL_S,
	C3_OBJTYPE_WALL_W,
	C3_OBJTYPE_SPRITE,
	C3_OBJTYPE_NUM
}		t_c3_object_type;

typedef struct	s_c3_hit_result
{
	t_c3_object_type	type;
	t_c3_coord			position;
}		t_c3_hit_result;

typedef struct	s_c3_ray
{
	double			distance;
	double			angle;
	t_c3_hit_result	hit;
}		t_c3_ray;

typedef struct	s_c3_renderer
{
	double		plane_distance;
	double		fov;
	int			resolution_x;
	int			resolution_y;
	int			minimap_width;
	int			minimap_height;
	t_c3_ray	*rays;
}		t_c3_renderer;

void	c3_renderer_cleanup(t_c3_renderer *rend)
{
	free(rend->rays);
}

typedef struct	s_c3_map
{
	char	*map;
	int		width;
	int		height;
}		t_c3_map;

void	c3_map_init(t_c3_map *map)
{
	static char	mapdata[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 1, 0, 1, 0, 0, 1,
		1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	const int width = 10;
	const int height = 10;

	map->map = mapdata;
	map->width = width;
	map->height = height;
}

typedef struct	s_c3_texture
{
	void	*image;
	char	*data;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}		t_c3_texture;

typedef struct	s_c3_texture_cache
{
	t_c3_texture	cache[C3_OBJTYPE_NUM];
}		t_c3_texture_cache;

typedef struct	s_c3_state
{
	void				*mlx;
	void				*window;
	int					screen_width;
	int					screen_height;
	void				*img;
	t_c3_imgdata		imgdata;
	t_c3_keystate		keystate;
	t_c3_player			player;
	t_c3_renderer		renderer;
	t_c3_map			map;
	t_c3_texture_cache	*texture_cache;
}		t_c3_state;

void	c3_player_init(t_c3_player *player, t_c3_map *map)
{
	player->x = map->width / 2.;
	player->y = map->height / 2.;
	player->direction = 0.;
	player->walk_speed = 0.01;
	player->rotation_speed = 0.01;
}

void	c3_renderer_init(t_c3_renderer *rend, int minimap_width, int minimap_height)
{
	rend->plane_distance = 1.;
	rend->fov = M_PI / 6.;
	rend->resolution_x = 320;
	rend->resolution_y = 180;
	rend->rays = malloc(sizeof(t_c3_renderer) * rend->resolution_x);
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

void	c3_log(const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void	c3_check(int64_t val, const char *message)
{
	if (!val)
	{
		c3_log("Check failed: %s\n", message);
		exit(1);
	}
}

void	c3_terminate(t_c3_state *stat)
{
	int	tmp;

	c3_log("%s: %p\n", __FUNCTION__, stat);
	tmp = mlx_do_key_autorepeaton(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeaton() returned false.");

	mlx_destroy_window(stat->mlx, stat->window);
	if (stat->img)
		mlx_destroy_image(stat->mlx, stat->img);

	mlx_loop_end(stat->mlx);

	mlx_destroy_display(stat->mlx);
	c3_renderer_cleanup(&stat->renderer);

	int	i;
	i = 0;
	while (i < C3_OBJTYPE_NUM)
	{
		free(stat->texture_cache->cache[i].image);
		i++;
	}
}

int		c3_key_press_hook(int key, void *param)
{
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	c3_log("%s: %02x : %p\n", __FUNCTION__, key, param);

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

	c3_log("%s: %02x : %p\n", __FUNCTION__, key, param);
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
	int x = stat->player.x * stat->renderer.minimap_width / stat->map.width;
	int y = stat->player.y * stat->renderer.minimap_height / stat->map.height;
	double angle = stat->player.direction;

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
				unsigned int col = mlx_get_color_value(
					stat->mlx, (0 << 24) + (0 << 16) + (0 << 8));
				stat->imgdata.data[index + 0] = (col >> 24) & 0xff;
				stat->imgdata.data[index + 1] = (col >> 16) & 0xff;
				stat->imgdata.data[index + 2] = (col >> 8) & 0xff;
				stat->imgdata.data[index + 3] = col & 0xff;
			}
		}
	}
}

void	c3_draw_map(t_c3_state *stat)
{
	for (int i = 0; i < stat->screen_height && i < stat->renderer.minimap_height; i++)
	{
		int y = stat->map.height * i / stat->renderer.minimap_height;
		for (int j = 0; j < stat->screen_width && j < stat->renderer.minimap_width; j++)
		{
			int x = stat->map.width * j / stat->renderer.minimap_width;
			int cell = stat->map.map[y * stat->map.width + x];

			/* int b = 255 * (1 - cell); */
			/* int g = j * 256 / stat->screen_width; */
			/* int r = i * 128 / stat->screen_height + 128; */
			int r = 128 * (1 - cell);
			int g = 128 * (1 - cell);
			int b = 128 * (1 - cell);

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

double	distance_squared(double x1, double y1, double x2, double y2)
{
	double	dx;
	double	dy;

	dx = x1 - x2;
	dy = y1 - y2;
	return (dx * dx + dy * dy);
}

void	c3_cast_ray(
	t_c3_state *stat, double x, double y, double theta, t_c3_hit_result *out)
{
	double	tan_theta;
	int		i;
	double	hori_hit_x;
	double	hori_hit_y;
	double	vert_hit_x;
	double	vert_hit_y;
	int		facing_north;
	int		facing_east;

	tan_theta = tan(theta);

	i = 1;
	if (tan_theta != 0.0)
		while (1)
		{
			if (theta >= 0 && theta < M_PI)
			{
				hori_hit_y = floor(y) + i;
				hori_hit_x = x + (hori_hit_y - y) / tan_theta;
				facing_north = 0;
				if (hori_hit_x < 0 || hori_hit_x >= stat->map.width
					|| hori_hit_y < 0 || hori_hit_y >= stat->map.height)
					break ;
				if (stat->map.map[(int)hori_hit_y * stat->map.width + (int)hori_hit_x])
					break ;
			}
			else
			{
				hori_hit_y = floor(y) - i + 1;
				hori_hit_x = x + (hori_hit_y - y) / tan_theta;
				facing_north = 1;
				if (hori_hit_x < 0 || hori_hit_x >= stat->map.width
					|| hori_hit_y < 1 || hori_hit_y >= stat->map.height + 1)
					break ;
				if (stat->map.map[(int)(hori_hit_y - 1) * stat->map.width + (int)hori_hit_x])
					break ;
			}
			i++;
		}

	i = 1;
	while (1)
	{
		if (theta < M_PI_2 || theta >= 3 * M_PI_2)
		{
			vert_hit_x = floor(x) + i;
			vert_hit_y = y + (vert_hit_x - x) * tan_theta;
			facing_east = 1;
			if (vert_hit_x < 0 || vert_hit_x >= stat->map.width
				|| vert_hit_y < 0 || vert_hit_y >= stat->map.height)
				break ;
			if (stat->map.map[(int)vert_hit_y * stat->map.width + (int)vert_hit_x])
				break ;
		}
		else
		{
			vert_hit_x = floor(x) - i + 1;
			vert_hit_y = y + (vert_hit_x - x) * tan_theta;
			facing_east = 0;
			if (vert_hit_x < 1 || vert_hit_x >= stat->map.width + 1
				|| vert_hit_y < 0 || vert_hit_y >= stat->map.height)
				break ;
			if (stat->map.map[(int)vert_hit_y * stat->map.width + (int)vert_hit_x - 1])
				break ;
		}
		i++;
	}

	if (distance_squared(x, y, hori_hit_x, hori_hit_y)
		< distance_squared(x, y, vert_hit_x, vert_hit_y))
	{
		out->position.x = hori_hit_x;
		out->position.y = hori_hit_y;
		out->type = facing_north ? C3_OBJTYPE_WALL_N : C3_OBJTYPE_WALL_S;
	}
	else
	{
		out->position.x = vert_hit_x;
		out->position.y = vert_hit_y;
		out->type = facing_east ? C3_OBJTYPE_WALL_E : C3_OBJTYPE_WALL_W;
	}
}

void	c3_draw_rays_on_map(t_c3_state *stat)
{
	double		world_x;
	double		world_y;
	double		screen_x;
	double		screen_y;
	int			x;

	x = 0;
	while (x < stat->renderer.resolution_x)
	{
		world_x = stat->renderer.rays[x].hit.position.x;
		world_y = stat->renderer.rays[x].hit.position.y;
		screen_x = world_x * stat->renderer.minimap_width / stat->map.width;
		screen_y = world_y * stat->renderer.minimap_height / stat->map.height;

		int r = 255 * x / stat->renderer.resolution_x;
		int col = (r << 16) + ((255 - r) << 0);
		mlx_string_put(
			stat->mlx, stat->window, screen_x, screen_y,
			mlx_get_color_value(stat->mlx, col), "*");

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

	printf("texture loaded: %s\n", path);
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
	};

	cache = stat->texture_cache;
	tex = &cache->cache[type];
	if (!tex->image)
		c3_texture_cache_load(stat, type, path[type]);

	int	index = v * tex->size_line + u * tex->bits_per_pixel / 8;

	texcol = ((uint32_t*)tex->data)[index / 4];

	return (texcol);
}

void	c3_draw_walls(t_c3_state *stat)
{
	int				x;
	int				y;
	t_c3_ray		*ray;
	unsigned int	col;
	int				screen_y;

	uint32_t	ceiling_r = 0x88;
	uint32_t	ceiling_g = 0x88;
	uint32_t	ceiling_b = 0xff;

	uint32_t	floor_r = 0x00;
	uint32_t	floor_g = 0x88;
	uint32_t	floor_b = 0x88;

	x = 0;
	while (x < stat->renderer.resolution_x)
	{
		ray = &stat->renderer.rays[x];

		int wall_height = stat->renderer.resolution_y / (ray->distance * cos(ray->angle));

		y = 0;
		while (y < stat->renderer.resolution_y)
		{
			int		v = c3_texture_size *
				(y - (stat->renderer.resolution_y - wall_height) / 2) /
				wall_height;
			if (y < (stat->renderer.resolution_y - wall_height) / 2)
				col = mlx_get_color_value(
					stat->mlx, (ceiling_r << 16) + (ceiling_g << 8) + ceiling_b);
			else if (y > (stat->renderer.resolution_y + wall_height) / 2)
				col = mlx_get_color_value(
					stat->mlx, (floor_r << 16) + (floor_g << 8) + floor_b);

			else
			{
				if (ray->hit.type == C3_OBJTYPE_WALL_N)
					col = c3_sample_texture(
						stat,
						C3_OBJTYPE_WALL_N,
						(int)(ray->hit.position.x * c3_texture_size) % c3_texture_size,
						v);

				else if (ray->hit.type == C3_OBJTYPE_WALL_E)
					col = c3_sample_texture(
						stat,
						C3_OBJTYPE_WALL_E,
						(int)(ray->hit.position.y * c3_texture_size) % c3_texture_size,
						v);

				else if (ray->hit.type == C3_OBJTYPE_WALL_S)
					col = c3_sample_texture(
						stat,
						C3_OBJTYPE_WALL_S,
						c3_texture_size - (int)(ray->hit.position.x * c3_texture_size) % c3_texture_size,
						v);

				else //(ray->hit.type == C3_OBJTYPE_WALL_W)
					col = c3_sample_texture(
						stat,
						C3_OBJTYPE_WALL_W,
						c3_texture_size - (int)(ray->hit.position.y * c3_texture_size) % c3_texture_size,
						v);

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

void	c3_draw(t_c3_state *stat)
{
	c3_draw_walls(stat);
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
		c3_cast_ray(stat, stat->player.x, stat->player.y, angle,
					&stat->renderer.rays[x + half_res].hit);
		double sq_dist = distance_squared(
			stat->player.x, stat->player.y,
			stat->renderer.rays[x + half_res].hit.position.x,
			stat->renderer.rays[x + half_res].hit.position.y);
		double distance = sqrt(sq_dist);
		stat->renderer.rays[x + half_res].distance = distance;
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
		double new_x = stat->player.x + delta_x * (stat->keystate.w ? 1 : -1);
		double new_y = stat->player.y + delta_y * (stat->keystate.w ? 1 : -1);
		if (new_x >= 0 && new_x < stat->map.width
			&& new_y >= 0 && new_y < stat->map.height)
		{
			stat->player.x = new_x;
			stat->player.y = new_y;
		}
	}

	if (stat->keystate.a || stat->keystate.d)
	{
		double delta_x = stat->player.walk_speed * sin(stat->player.direction);
		double delta_y = stat->player.walk_speed * -cos(stat->player.direction);
		double new_x = stat->player.x + delta_x * (stat->keystate.a ? 1 : -1);
		double new_y = stat->player.y + delta_y * (stat->keystate.a ? 1 : -1);
		if (new_x >= 0 && new_x < stat->map.width
			&& new_y >= 0 && new_y < stat->map.height)
		{
			stat->player.x = new_x;
			stat->player.y = new_y;
		}
	}

	c3_scan(stat);
}

int		c3_loop_hook(void *param)
{
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
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

int		c3_init(t_c3_state *stat, t_c3_texture_cache *tex)
{
	int			tmp;

	c3_map_init(&stat->map);

	stat->screen_width = 1280;
	stat->screen_height = 720;
	c3_keystate_init(&stat->keystate);
	c3_player_init(&stat->player, &stat->map);
	c3_renderer_init(&stat->renderer, stat->map.width * 16, stat->map.height * 16);

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

	stat->img = mlx_new_image(
		stat->mlx, stat->screen_width, stat->screen_height);
	C3_CHECK(stat->img, "mlx_new_image() returned NULL.");
	stat->imgdata.data = mlx_get_data_addr(
		stat->img,
		&stat->imgdata.bits_per_pixel,
		&stat->imgdata.size_line,
		&stat->imgdata.endian);

	c3_log(
		"bpp: %d, linesize: %d, endian: %d\n",
		stat->imgdata.bits_per_pixel,
		stat->imgdata.size_line, stat->imgdata.endian);

	tmp = mlx_loop_hook(stat->mlx, c3_loop_hook, stat);
	C3_CHECK(tmp, "mlx_loop_hook() returned false.");

	c3_texture_cache_init(tex);
	stat->texture_cache = tex;

	tmp = mlx_do_key_autorepeatoff(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeatoff() returned false.");

	return (1);
}

int		main(void)
{
	t_c3_state			stat;
	t_c3_texture_cache	tex;

	c3_init(&stat, &tex);

	mlx_loop(stat.mlx);
}
