/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 19:00:48 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:37:02 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void	*mem;
	size_t	total_size;

	total_size = number * size;
	mem = malloc(total_size);
	if (mem)
		ft_bzero(mem, total_size);
	return (mem);
}
