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

#include "bmp.h"

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
