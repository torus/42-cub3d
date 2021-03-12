/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 20:13:36 by thisai            #+#    #+#             */
/*   Updated: 2021/03/11 20:13:36 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/XKBlib.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

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
