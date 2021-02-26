/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 20:38:21 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:41:51 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *b1, const void *b2, size_t len)
{
	int	d;

	d = 0;
	while (len--)
	{
		d = (int)*(unsigned char*)b1++ - *(unsigned char*)b2++;
		if (d)
			break ;
	}
	return (d);
}
