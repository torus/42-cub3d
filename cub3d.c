/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:23:13 by thisai            #+#    #+#             */
/*   Updated: 2021/01/13 18:33:45 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>

#include <stdarg.h>
#include <stdlib.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <mlx.h>

#define C3_CHECK(val, mesg) c3_check((int64_t)val, mesg)

typedef struct	s_c3_imgdata
{
	char	*data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}		t_c3_imgdata;

typedef struct	s_c3_state
{
	void			*mlx;
	void			*window;
	int				screen_width;
	int				screen_height;
	void			*img;
	t_c3_imgdata	imgdata;
}		t_c3_state;

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

void	c3_cleanup(t_c3_state *stat)
{
	mlx_destroy_window(stat->mlx, stat->window);
	if (stat->img)
		mlx_destroy_image(stat->mlx, stat->img);
}

int		c3_key_hook(int key, void *param)
{
	t_c3_state	*stat;

	c3_log("%s: %02x : %p\n", __FUNCTION__, key, param);
	stat = (t_c3_state*)param;
	if (key == XK_Escape)
	{
		c3_cleanup(stat);
		exit(0);
	}
	return (0);
}

int		c3_init(t_c3_state *stat)
{
	int			tmp;

	stat->screen_width = 640;
	stat->screen_height = 480;
	stat->mlx = mlx_init();
	C3_CHECK(stat->mlx, "mlx is NULL.");
	stat->window = mlx_new_window(
		stat->mlx, stat->screen_width, stat->screen_height, "Cub3D!");
	C3_CHECK(stat->window, "window is NULL.");

	tmp = mlx_key_hook(stat->window, c3_key_hook, stat);
	C3_CHECK(tmp, "mlx_key_hook() returned false.");

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

	return (1);
}

int		main(void)
{
	t_c3_state	stat;

	c3_init(&stat);

	mlx_loop(stat.mlx);
}
