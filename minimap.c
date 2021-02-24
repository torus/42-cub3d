/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 18:20:58 by thisai            #+#    #+#             */
/*   Updated: 2021/02/24 16:55:16 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

int		c3_minimap_lookup_player_pixel(int x, int y)
{
	static const char	bitmap[] = {
		8, 12, 254, 255, 254, 12, 8, 0
	};

	if (x >= 0 && x < 8 && y >= 0 && y < 8)
		return (bitmap[y] & (1 << (8 - x)));
	else
		return (0);
}

void	c3_minimap_put_pixel(t_c3_state *stat, int x, int y, uint32_t col)
{
	int	index;
	int	max_index;

	max_index = stat->screen_width * stat->imgdata.size_line;
	index =
		y * stat->imgdata.size_line +
		x * stat->imgdata.bits_per_pixel / 8;
	if (index >= 0 && index < max_index)
		((uint32_t*)stat->imgdata.data)[index / 4] = col;
}

void	c3_draw_player_on_map(t_c3_state *stat)
{
	int		x;
	int		y;
	double	angle;
	int		i;
	int		j;

	x = stat->player.position.x
		* stat->renderer.minimap_width / stat->map.width;
	y = stat->player.position.y
		* stat->renderer.minimap_height / stat->map.height;
	angle = stat->player.direction;
	i = -8;
	while (i < 9)
	{
		j = -8;
		while (j < 9)
		{
			if (c3_minimap_lookup_player_pixel(
					j * cos(-angle) - i * sin(-angle),
					j * sin(-angle) + i * cos(-angle) + 3))
				c3_minimap_put_pixel(stat, j + x, i + y, 255 << 16);
			j++;
		}
		i++;
	}
}

void	c3_draw_map(t_c3_state *stat)
{
	int	i;
	int	j;
	int	y;
	int	x;
	int	v;

	i = 0;
	while (i < stat->screen_height && i < stat->renderer.minimap_height)
	{
		y = stat->map.height * i / stat->renderer.minimap_height;
		j = 0;
		while (j < stat->screen_width && j < stat->renderer.minimap_width)
		{
			x = stat->map.width * j / stat->renderer.minimap_width;
			v = 127 * (1 - c3_query_map(stat, x, y)) + 128;
			((uint32_t*)stat->imgdata.data)[
				(i * stat->imgdata.size_line
				+ j * stat->imgdata.bits_per_pixel / 8)
				/ 4] = (v << 16) + (v << 8) + v;
			j++;
		}
		i++;
	}
	c3_draw_player_on_map(stat);
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
