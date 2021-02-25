/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 17:12:17 by thisai            #+#    #+#             */
/*   Updated: 2021/02/25 17:12:17 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>

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

int		c3_focusin_hook(void *param)
{
	int			tmp;
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	tmp = mlx_do_key_autorepeatoff(stat->mlx);
	c3_check(!!tmp, "mlx_do_key_autorepeatoff() returned false.");
	return (1);
}

int		c3_focusout_hook(void *param)
{
	int			tmp;
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	tmp = mlx_do_key_autorepeaton(stat->mlx);
	c3_check(!!tmp, "mlx_do_key_autorepeatoff() returned false.");
	return (1);
}

int		c3_client_hook(void *param)
{
	t_c3_state	*stat;

	stat = (t_c3_state*)param;
	c3_terminate(stat);
	exit(0);
}
