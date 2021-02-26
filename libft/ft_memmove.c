/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 18:46:00 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:40:18 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*p;

	if (src > dst)
	{
		p = dst;
		while (len--)
			*p++ = *(char*)src++;
	}
	else if (src < dst)
	{
		p = dst;
		while (len--)
			p[len] = ((char*)src)[len];
	}
	return (dst);
}
