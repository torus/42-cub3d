/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:39:55 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 11:04:34 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	load_to_buf(int n, int sign, char *buf)
{
	int	digits;

	digits = 0;
	while (n)
	{
		if (sign > 0)
			buf[digits++] = n % 10 + '0';
		else
			buf[digits++] = '0' - n % 10;
		n /= 10;
	}
	if (sign < 0)
		buf[digits++] = '-';
	return (digits);
}

static char	*zero(void)
{
	char	*dest;

	dest = malloc(2);
	if (dest)
		ft_strlcpy(dest, "0", 2);
	return (dest);
}

static char	*make_str(int digits, char *buf)
{
	char	*dest;
	int		i;

	dest = malloc(digits + 1);
	if (dest)
	{
		i = 0;
		while (i < digits)
		{
			dest[i] = buf[digits - i - 1];
			i++;
		}
		dest[i] = '\0';
	}
	return (dest);
}

char		*ft_itoa(int n)
{
	char	buf[32];
	int		sign;
	int		digits;

	if (n == 0)
		return (zero());
	if (n < 0)
		sign = -1;
	else
		sign = 1;
	digits = load_to_buf(n, sign, buf);
	return (make_str(digits, buf));
}
