/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/24 17:45:42 by thisai            #+#    #+#             */
/*   Updated: 2020/10/24 17:45:43 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	srclen;
	char	*dest;

	srclen = ft_strlen(s);
	if (srclen < start)
		len = 0;
	if (len > srclen - start)
		len = srclen - start;
	dest = malloc(len + 1);
	if (dest)
		ft_strlcpy(dest, s + start, len + 1);
	return (dest);
}
