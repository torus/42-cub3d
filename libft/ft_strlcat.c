/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 15:20:04 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:46:38 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	srclen;
	size_t	dstlen;
	char	*p;

	srclen = ft_strlen(src);
	p = dst;
	while (*p && size > 0)
	{
		size--;
		p++;
	}
	dstlen = p - dst;
	while (size > 1 && *src)
	{
		*p++ = *src++;
		size--;
	}
	if (size > 0)
		*p = '\0';
	return (srclen + dstlen);
}
