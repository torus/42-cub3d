/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loophook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:24:40 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 12:24:40 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

void	c3_draw(t_c3_state *stat)
{
	c3_render_scene(stat);
	if (stat->is_drawing_minimap)
		c3_draw_map(stat);
	if (stat->is_showing_screen)
		mlx_put_image_to_window(stat->mlx, stat->window, stat->img, 0, 0);
}

void	c3_handle_side_move(t_c3_state *stat)
{
	double delta_x;
	double delta_y;
	double new_x;
	double new_y;

	delta_x = stat->player.walk_speed * cos(stat->player.direction);
	delta_y = stat->player.walk_speed * sin(stat->player.direction);
	new_x = stat->player.position.x + delta_x * (stat->keystate.w ? 1 : -1);
	new_y = stat->player.position.y + delta_y * (stat->keystate.w ? 1 : -1);
	if (new_x >= 0 && new_x < stat->map.width
		&& new_y >= 0 && new_y < stat->map.height)
	{
		stat->player.position.x = new_x;
		stat->player.position.y = new_y;
	}
}

void	c3_handle_forward_move(t_c3_state *stat)
{
	double delta_x;
	double delta_y;
	double new_x;
	double new_y;

	delta_x = stat->player.walk_speed * sin(stat->player.direction);
	delta_y = stat->player.walk_speed * -cos(stat->player.direction);
	new_x = stat->player.position.x + delta_x * (stat->keystate.a ? 1 : -1);
	new_y = stat->player.position.y + delta_y * (stat->keystate.a ? 1 : -1);
	if (new_x >= 0 && new_x < stat->map.width
		&& new_y >= 0 && new_y < stat->map.height)
	{
		stat->player.position.x = new_x;
		stat->player.position.y = new_y;
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
		c3_handle_side_move(stat);
	if (stat->keystate.a || stat->keystate.d)
		c3_handle_forward_move(stat);
	c3_scan(stat);
}

int		c3_loop_hook(void *param)
{
	t_c3_state	*stat;
	static int	current_frame;

	stat = (t_c3_state*)param;
	if (stat->is_benchmarking && current_frame++ > 1000)
	{
		c3_terminate(stat);
		exit(0);
	}
	c3_update(stat);
	c3_draw(stat);
	return (1);
}
