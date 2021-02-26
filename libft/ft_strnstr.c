/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 17:27:32 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:48:14 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	test_substring(const char *big, const char *little, size_t len)
{
	while (1)
	{
		if (!*little)
			return (1);
		if (*big != *little || len == 0)
			return (0);
		big++;
		little++;
		len--;
	}
}

char		*ft_strnstr(const char *big, const char *little, size_t len)
{
	if (!*little)
		return ((char*)big);
	while (len > 0)
	{
		if (test_substring(big, little, len))
			return ((char*)big);
		big++;
		len--;
	}
	return (NULL);
}
