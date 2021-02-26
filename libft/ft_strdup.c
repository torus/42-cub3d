/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 19:06:05 by thisai            #+#    #+#             */
/*   Updated: 2020/10/18 19:10:11 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *str)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(str);
	dest = malloc(size + 1);
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, str, size + 1);
	return (dest);
}
