/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 09:43:52 by thisai            #+#    #+#             */
/*   Updated: 2021/02/10 09:45:59 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>

#include "cub3d.h"

void	c3_log(const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}
