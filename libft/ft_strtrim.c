/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/24 18:11:43 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:51:50 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned char		table[256];
	const char			*first;
	const char			*last;
	const unsigned char	*p;
	char				*dest;

	ft_bzero(table, 256);
	while (*set)
		table[*(unsigned char*)set++] = 1;
	p = (const unsigned char*)s1;
	while (*p && table[*p])
		p++;
	first = (const char*)p;
	while (*p)
	{
		if (!table[*p])
			last = (const char*)p;
		p++;
	}
	if (!(dest = malloc(*first ? last - first + 2 : 1)))
		return (NULL);
	if (*first)
		ft_memcpy(dest, first, last - first + 1);
	dest[*first ? last - first + 1 : 0] = '\0';
	return (dest);
}
