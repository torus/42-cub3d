/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 18:20:58 by thisai            #+#    #+#             */
/*   Updated: 2021/02/21 18:20:58 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

void	c3_draw_player_on_map(t_c3_state *stat)
{
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
