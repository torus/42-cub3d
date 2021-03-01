/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 17:37:36 by thisai            #+#    #+#             */
/*   Updated: 2021/02/25 17:37:36 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "cub3d.h"
#include "cub3d_int.h"
#include "scene.h"

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
			C3_LOG("Error\nInvalid map.\n");
			c3_scene_cleanup(scene);
			exit(1);
		}
		i++;
	}
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
	C3_LOG("Error\nThe map is not closed.\n");
	c3_terminate(stat);
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
