/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 21:19:49 by thisai            #+#    #+#             */
/*   Updated: 2020/10/29 10:43:46 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

static void	reverse(char *buf, int length)
{
	char	*last;
	char	tmp;

	last = buf + length - 1;
	while (last > buf)
	{
		tmp = *buf;
		*buf = *last;
		*last = tmp;
		buf++;
		last--;
	}
}

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
	reverse(buf, digits);
	return (digits);
}

void		ft_putnbr_fd(int n, int fd)
{
	char	buf[32];
	int		sign;
	int		digits;

	if (n == 0)
	{
		write(fd, "0", 2);
		return ;
	}
	if (n < 0)
		sign = -1;
	else
		sign = 1;
	digits = load_to_buf(n, sign, buf);
	write(fd, buf, digits);
}
