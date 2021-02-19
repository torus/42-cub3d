/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_storage.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 15:44:14 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 15:44:14 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "scene.h"
#include "scene_int.h"

void	c3_strbuf_ungetc(t_c3_scene_container cont)
{
	t_c3_strbuf	*buf;

	buf = cont.strbuf;
	if (buf->index > 0)
		buf->index--;
}

int		c3_strbuf_getc(t_c3_scene_container cont)
{
	t_c3_strbuf	*buf;
	char		ch;

	buf = cont.strbuf;
	ch = buf->str[buf->index];
	if (ch)
	{
		buf->index++;
		return (ch);
	}
	return (-1);
}

void	c3_file_ungetc(t_c3_scene_container cont)
{
	t_c3_file	*file;

	file = cont.file;
	if (file->is_ungotten)
	{
		c3_log("ungetc: too many chars ungotten.");
		return ;
	}
	file->is_ungotten = 1;
	file->ungotten = file->last_char;
}

int		c3_file_getc(t_c3_scene_container cont)
{
	t_c3_file	*file;
	char		buf[1];
	int			read_result;

	file = cont.file;
	if (file->is_ungotten)
	{
		file->is_ungotten = 0;
		file->last_char = file->ungotten;
		return (file->ungotten);
	}
	read_result = read(file->fd, buf, 1);
	if (read_result == 0)
		return (-1);
	if (read_result < 0)
	{
		perror("read failed");
		return (-1);
	}
	file->last_char = buf[0];
	return (buf[0]);
}
