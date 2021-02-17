/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmptest.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 15:35:25 by thisai            #+#    #+#             */
/*   Updated: 2021/02/17 15:35:25 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "../bmp.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 320
#define BPP 32

int	main()
{
	unsigned char	dest[6];

	c3_bmp_put_int16(dest, 0xdead);
	c3_bmp_put_int32(dest + 2, 0xc001cafe);

	assert(dest[0] == 0xad);
	assert(dest[1] == 0xde);
	assert(dest[2] == 0xfe);
	assert(dest[3] == 0xca);
	assert(dest[4] == 0x01);
	assert(dest[5] == 0xc0);





	t_bitmap_file_header	file_header;
	t_bitmap_info_header	info_header;
	uint32_t				image_data[IMAGE_WIDTH * IMAGE_HEIGHT];

	file_header.file_type[0] = 'B';
	file_header.file_type[1] = 'M';

	c3_bmp_put_int32(&file_header.file_size,
					 sizeof(file_header) + sizeof(info_header) +
					 sizeof(image_data));
	c3_bmp_put_int16(&file_header.reserved1, 0);
	c3_bmp_put_int16(&file_header.reserved2, 0);
	c3_bmp_put_int32(&file_header.offset, sizeof(file_header) + sizeof(info_header));

	c3_bmp_put_int32(&info_header.header_size, 40);


	c3_bmp_put_int32(&info_header.image_width, IMAGE_WIDTH);
	c3_bmp_put_int32(&info_header.image_height, -IMAGE_HEIGHT);

	c3_bmp_put_int16(&info_header.planes, 1);

	c3_bmp_put_int16(&info_header.bpp, BPP);
	c3_bmp_put_int32(&info_header.compression, 0);
	c3_bmp_put_int32(&info_header.image_size, sizeof(image_data));
	c3_bmp_put_int32(&info_header.resolution_x, 3780);
	c3_bmp_put_int32(&info_header.resolution_y, 3780);
	c3_bmp_put_int32(&info_header.num_colors, 0);
	c3_bmp_put_int32(&info_header.num_important_colors, 0);

	for (int i = 0; i < IMAGE_HEIGHT; i++)
	{
		for (int j = 0; j < IMAGE_WIDTH; j++)
		{
			image_data[i * IMAGE_WIDTH + j] =
				((256 * j / IMAGE_WIDTH) << 16) +
				((256 * i / IMAGE_HEIGHT) << 8) + 128;
		}
	}

	write(1, &file_header, sizeof(file_header));
	write(1, &info_header, sizeof(info_header));
	write(1, image_data, sizeof(image_data));
}
