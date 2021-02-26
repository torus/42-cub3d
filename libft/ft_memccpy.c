/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 18:24:29 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 11:04:19 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	ch;

	while (n--)
	{
		ch = *(unsigned char*)src++;
		*(unsigned char*)dst++ = ch;
		if (ch == (unsigned char)c)
			return (dst);
	}
	return (NULL);
}
