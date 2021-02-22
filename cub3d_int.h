/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:30:44 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 12:30:44 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_INT_H
# define CUB3D_INT_H

#include <stdint.h>
#include "cub3d.h"

void	c3_update(t_c3_state *stat);
void	c3_draw(t_c3_state *stat);

typedef struct s_bitmap_file_header
{
	unsigned char	file_type[2];
	unsigned char	file_size[4];
	unsigned char	reserved1[2];
	unsigned char	reserved2[2];
	unsigned char	offset[4];
}	t_bitmap_file_header;

typedef struct	s_bitmap_info_header
{
	unsigned char	header_size[4];
	unsigned char	image_width[4];
	unsigned char	image_height[4];
	unsigned char	planes[2];
	unsigned char	bpp[2];
	unsigned char	compression[4];
	unsigned char	image_size[4];
	unsigned char	resolution_x[4];
	unsigned char	resolution_y[4];
	unsigned char	num_colors[4];
	unsigned char	num_important_colors[4];
}	t_bitmap_info_header;

void	c3_bmp_put_int16(void *dest, int16_t val);
void	c3_bmp_put_int32(void *dest, int32_t val);
void	c3_bmp_generate(t_c3_state *stat);

#endif
