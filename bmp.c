/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 15:29:18 by thisai            #+#    #+#             */
/*   Updated: 2021/02/17 15:29:18 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "cub3d_int.h"

void	c3_bmp_put_int16(void *dest, int16_t val)
{
	unsigned char	*p;

	p = dest;
	p[1] = val >> 8 & 0xff;
	p[0] = val & 0xff;
}

void	c3_bmp_put_int32(void *dest, int32_t val)
{
	c3_bmp_put_int16(dest + 2, val >> 16);
	c3_bmp_put_int16(dest, val);
}

void	c3_bmp_set_header(
	t_c3_state *stat,
	t_bitmap_file_header *file_header, t_bitmap_info_header *info_header,
	size_t data_size)
{
	size_t	header_size;

	header_size = sizeof(t_bitmap_file_header) + sizeof(t_bitmap_info_header);
	file_header->file_type[0] = 'B';
	file_header->file_type[1] = 'M';
	c3_bmp_put_int32(&file_header->file_size, header_size + data_size);
	c3_bmp_put_int16(&file_header->reserved1, 0);
	c3_bmp_put_int16(&file_header->reserved2, 0);
	c3_bmp_put_int32(&file_header->offset, header_size);
	c3_bmp_put_int32(&info_header->header_size, sizeof(t_bitmap_info_header));
	c3_bmp_put_int32(&info_header->image_width, stat->screen_width);
	c3_bmp_put_int32(&info_header->image_height, -stat->screen_height);
	c3_bmp_put_int16(&info_header->planes, 1);
	c3_bmp_put_int16(&info_header->bpp, 32);
	c3_bmp_put_int32(&info_header->compression, 0);
	c3_bmp_put_int32(&info_header->image_size, data_size);
	c3_bmp_put_int32(&info_header->resolution_x, 3780);
	c3_bmp_put_int32(&info_header->resolution_y, 3780);
	c3_bmp_put_int32(&info_header->num_colors, 0);
	c3_bmp_put_int32(&info_header->num_important_colors, 0);
}

void	c3_bmp_generate(t_c3_state *stat)
{
	t_bitmap_file_header	file_header;
	t_bitmap_info_header	info_header;
	size_t					data_size;
	int						fd;

	c3_update(stat);
	c3_draw(stat);
	data_size = stat->screen_height * stat->imgdata.size_line;
	c3_bmp_set_header(stat, &file_header, &info_header, data_size);
	fd = open("out.bmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		c3_log("Error\n%s\n", strerror(errno));
		exit(1);
	}
	write(fd, &file_header, sizeof(file_header));
	write(fd, &info_header, sizeof(info_header));
	write(fd, stat->imgdata.data, data_size);
}
