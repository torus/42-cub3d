/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 16:44:29 by thisai            #+#    #+#             */
/*   Updated: 2021/02/25 16:44:29 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <mlx.h>
#include "cub3d.h"
#include "cub3d_int.h"

void		c3_texture_cache_load(
	t_c3_state *stat, t_c3_object_type type)
{
	int				width;
	int				height;
	void			*image;
	t_c3_texture	*tex;
	char			*path;

	tex = &stat->texture_cache->cache[type];
	path = stat->texture_cache->path[type];
	image = mlx_xpm_file_to_image(stat->mlx, path, &width, &height);
	if (!image)
	{
		C3_LOG("Error\ntexture not found.\n");
		c3_terminate(stat);
		exit(1);
	}
	tex->image = image;
	tex->width = width;
	tex->height = height;
	stat->texture_cache->cache[type].data = mlx_get_data_addr(
		image, &tex->bits_per_pixel, &tex->size_line, &tex->endian);
}

uint32_t	c3_sample_texture(
	t_c3_state *stat, t_c3_object_type type, double u, double v)
{
	uint32_t			texcol;
	t_c3_texture_cache	*cache;
	t_c3_texture		*tex;
	int					index;

	cache = stat->texture_cache;
	tex = &cache->cache[type];
	if (!tex->image)
		c3_texture_cache_load(stat, type);
	index = (int)(v * tex->height) * tex->size_line
		+ u * tex->width * tex->bits_per_pixel / 8;
	texcol = ((uint32_t*)tex->data)[index / 4];
	return (texcol);
}
