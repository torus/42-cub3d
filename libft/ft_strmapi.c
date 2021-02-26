/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 20:19:36 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:47:17 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	len;
	char	*dest;

	len = ft_strlen(s);
	dest = malloc(len + 1);
	if (dest)
	{
		dest[len] = '\0';
		while (len--)
			dest[len] = f(len, s[len]);
	}
	return (dest);
}
