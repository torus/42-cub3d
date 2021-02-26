/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/24 17:47:21 by thisai            #+#    #+#             */
/*   Updated: 2020/10/24 17:51:21 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*dest;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	dest = malloc(len1 + len2 + 1);
	if (dest)
	{
		ft_memcpy(dest, s1, len1);
		ft_memcpy(dest + len1, s2, len2);
		dest[len1 + len2] = '\0';
	}
	return (dest);
}
