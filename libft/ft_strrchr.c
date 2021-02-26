/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 13:33:13 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:49:21 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*p;
	char		*dest;

	dest = NULL;
	while (1)
	{
		p = s++;
		if ((unsigned char)*p == c)
			dest = (char*)p;
		if (!*p)
			break ;
	}
	return (dest);
}
