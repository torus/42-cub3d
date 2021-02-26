/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 10:03:42 by thisai            #+#    #+#             */
/*   Updated: 2020/11/01 10:11:25 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"

#define CUTOFF (LONG_MAX / 10)
#define CUTLIM (LONG_MAX % 10)
#define CUTOFFN (-(unsigned long)LONG_MIN / 10)
#define CUTLIMN (-(unsigned long)LONG_MIN % 10)

static int	ft_isspace(char ch)
{
	return (ch == ' '
			|| ch == '\f'
			|| ch == '\n'
			|| ch == '\r'
			|| ch == '\t'
			|| ch == '\v');
}

int			ft_atoi(const char *nptr)
{
	int				sign;
	unsigned long	num;
	unsigned int	digit;

	sign = 1;
	num = 0;
	while (*nptr && ft_isspace(*nptr))
		nptr++;
	if (*nptr && (*nptr == '-' || *nptr == '+'))
		if (*nptr++ == '-')
			sign = -1;
	while (*nptr && (*nptr >= '0' && *nptr <= '9'))
	{
		digit = *nptr - '0';
		if (sign > 0 && (num > CUTOFF || (num == CUTOFF && digit > CUTLIM)))
			return (-1);
		if (sign < 0 && (num > CUTOFFN || (num == CUTOFFN && digit > CUTLIMN)))
			return (0);
		num = num * 10 + digit;
		nptr++;
	}
	return ((int)(sign * num));
}
