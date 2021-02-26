/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 13:51:26 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:46:12 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t len)
{
	while (len--)
	{
		if (*s1 == *s2)
		{
			if (*s1 == '\0')
				return (0);
			s1++;
			s2++;
		}
		else if ((unsigned char)*s1 > (unsigned char)*s2)
			return (1);
		else
			return (-1);
	}
	return (0);
}
