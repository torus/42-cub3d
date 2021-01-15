/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:23:13 by thisai            #+#    #+#             */
/*   Updated: 2021/01/15 17:50:26 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include <stdarg.h>
#include <stdlib.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

#include <mlx.h>

#define C3_CHECK(val, mesg) c3_check((int64_t)val, mesg)

const char	c3_map[] = {
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

const int map_width = 10;
const int map_height = 10;
const int block_width = 32;

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

typedef struct	s_c3_state
{
	void			*mlx;
	void			*window;
	int				screen_width;
	int				screen_height;
	void			*img;
	t_c3_imgdata	imgdata;
	t_c3_keystate	keystate;
	t_c3_player		player;
}		t_c3_state;

void	c3_player_init(t_c3_player *player)
{
	player->x = map_width * block_width / 2.;
	player->y = map_height * block_width / 2.;
	player->direction = 0.;
	player->walk_speed = 0.2;
	player->rotation_speed = 0.03;
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
	int x = stat->player.x * stat->screen_width / (map_width * block_width);
	int y = stat->player.y * stat->screen_height / (map_height * block_width);
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
	for (int i = 0; i < stat->screen_height; i++)
	{
		int y = map_height * i / stat->screen_height;
		for (int j = 0; j < stat->screen_width; j++)
		{
			int x = map_width * j / stat->screen_width;
			int cell = c3_map[y * map_width + x];

			int b = 255 * (1 - cell);
			int g = j * 256 / stat->screen_width;
			int r = i * 128 / stat->screen_height + 128;

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

void	c3_draw(t_c3_state *stat)
{
	c3_draw_map(stat);

	mlx_put_image_to_window(stat->mlx, stat->window, stat->img, 0, 0);

	mlx_string_put(
		stat->mlx, stat->window, 10, 10, mlx_get_color_value(stat->mlx, 0xffffff), "AHO");
}

void	c3_update(t_c3_state *stat)
{
	if (stat->keystate.left)
	{
		stat->player.direction -= stat->player.rotation_speed;
	}
	else if (stat->keystate.right)
	{
		stat->player.direction += stat->player.rotation_speed;
	}
	if (stat->keystate.w || stat->keystate.s)
	{
		double delta_x = stat->player.walk_speed * cos(stat->player.direction);
		double delta_y = stat->player.walk_speed * sin(stat->player.direction);
		double new_x = stat->player.x + delta_x * (stat->keystate.w ? 1 : -1);
		double new_y = stat->player.y + delta_y * (stat->keystate.w ? 1 : -1);
		if (new_x >= 0 && new_x < map_width * block_width
			&& new_y >= 0 && new_y < map_height * block_width)
		{
			stat->player.x = new_x;
			stat->player.y = new_y;
		}
	}
}

int		c3_loop_hook(void *param)
{
	t_c3_state	*stat;
 
	stat = (t_c3_state*)param;
	c3_update(stat);
	c3_draw(stat);
	return (1);
}

int		c3_init(t_c3_state *stat)
{
	int			tmp;

	stat->screen_width = 640;
	stat->screen_height = 480;
	c3_keystate_init(&stat->keystate);
	c3_player_init(&stat->player);

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






	tmp = mlx_do_key_autorepeatoff(stat->mlx);
	C3_CHECK(tmp, "mlx_do_key_autorepeatoff() returned false.");

	return (1);
}

int		main(void)
{
	t_c3_state	stat;

	c3_init(&stat);

	mlx_loop(stat.mlx);
}
