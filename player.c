/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 17:47:16 by thisai            #+#    #+#             */
/*   Updated: 2021/02/25 17:47:16 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "cub3d_int.h"

double	c3_get_direction_from_symbol(char ch)
{
	if (ch == 'N')
		return (-M_PI_2);
	else if (ch == 'E')
		return (0);
	else if (ch == 'S')
		return (M_PI_2);
	return (M_PI);
}

int		c3_player_set_initial_position(t_c3_player *player, t_c3_map *map)
{
	int		i;
	int		init_pos_found;
	char	ch;

	init_pos_found = 0;
	i = 0;
	while (i < map->width * map->height)
	{
		ch = map->map[i];
		if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
		{
			if (init_pos_found)
			{
				C3_LOG("Error\nMultiple start position contained in the map\n");
				exit(1);
			}
			init_pos_found = 1;
			player->direction = c3_get_direction_from_symbol(ch);
			player->position.x = i % map->width + 0.5;
			player->position.y = i / map->width + 0.5;
		}
		i++;
	}
	return (init_pos_found);
}

void	c3_player_init(t_c3_player *player, t_c3_map *map)
{
	int		init_pos_found;

	init_pos_found = c3_player_set_initial_position(player, map);
	if (!init_pos_found)
	{
		C3_LOG("Error\nStart position not found in the map\n");
		exit(1);
	}
	player->walk_speed = 0.01;
	player->rotation_speed = 0.01;
}
