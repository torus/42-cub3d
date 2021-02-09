/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 14:12:59 by thisai            #+#    #+#             */
/*   Updated: 2021/02/08 20:42:32 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cubfile.h"

int	c3_scene_read_int(t_c3_scene_buffer *buf)
{
	int		result;
	char	ch;

	ch = buf->getc(buf->container);

	result = 0;
	while (ch >= '0' && ch <= '9')
	{
		result = result * 10 + (ch - '0');
		ch = buf->getc(buf->container);
	}
	buf->ungetc(buf->container);
	return (result);
}

t_c3_token c3_scene_get_token(t_c3_scene_buffer *buf)
{
	int	ch;

	ch = buf->getc(buf->container);
	while (ch == '\n')
	{
		buf->is_beginning_of_line = 1;
		ch = buf->getc(buf->container);
	}

	if (buf->is_beginning_of_line)
	{
		buf->is_beginning_of_line = 0;
		if (ch < 0)
			return (C3_SCENE_TOKEN_UNKNOWN);
		if (ch == 'R')
			return (C3_SCENE_TOKEN_R);
		if (ch == 'N')
		{
			ch = buf->getc(buf->container);
			if (ch == 'O')
				return (C3_SCENE_TOKEN_NO);
			buf->ungetc(buf->container);
			return (C3_SCENE_TOKEN_UNKNOWN);
		}
		if (ch == 'S')
		{
			ch = buf->getc(buf->container);
			if (ch == 'O')
				return (C3_SCENE_TOKEN_SO);
			buf->ungetc(buf->container);
			if (ch == ' ')
				return (C3_SCENE_TOKEN_S);
			return (C3_SCENE_TOKEN_UNKNOWN);
		}
		if (ch == 'W')
		{
			ch = buf->getc(buf->container);
			if (ch == 'E')
				return (C3_SCENE_TOKEN_WE);
			buf->ungetc(buf->container);
			return (C3_SCENE_TOKEN_UNKNOWN);
		}
		if (ch == 'E')
		{
			ch = buf->getc(buf->container);
			if (ch == 'A')
				return (C3_SCENE_TOKEN_EA);
			buf->ungetc(buf->container);
			return (C3_SCENE_TOKEN_UNKNOWN);
		}
		if (ch == 'F')
			return (C3_SCENE_TOKEN_F);
		if (ch == 'C')
			return (C3_SCENE_TOKEN_C);
		return (C3_SCENE_TOKEN_POSSIBLY_MAP);
	}

	while (ch == ' ' || ch == '\t')
		ch = buf->getc(buf->container);

	if (ch >= '0' && ch <= '9')
	{
		buf->ungetc(buf->container);
		buf->int_value = c3_scene_read_int(buf);
		return (C3_SCENE_TOKEN_NUM);
	}

	if (ch == ',')
		return (C3_SCENE_TOKEN_COMMA);

	return (-1);
}

int			c3_scene_get_int(t_c3_scene_buffer *buf)
{
	return (buf->int_value);
}

const char*	c3_scene_get_string(t_c3_scene_buffer *buf)
{
	char	ch;
	int		index;

	ch = buf->getc(buf->container);
	while (ch == ' ' || ch == '\t')
		ch = buf->getc(buf->container);

	index = 0;
	while (ch > 0 && ch != '\n' && index < C3_STRING_BUFFER_SIZE - 1)
	{
		buf->string_value[index++] = ch;
		ch = buf->getc(buf->container);
	}
	buf->ungetc(buf->container);
	buf->string_value[index] = '\0';
	return (buf->string_value);
}

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

t_c3_parse_result	c3_scene_parse_resolution(t_c3_scene *scene, t_c3_scene_buffer *buf)
{
	int			width;
	int			height;

	if (c3_scene_get_token(buf) != C3_SCENE_TOKEN_NUM)
		return (C3_PARSE_FAIL);
	width = c3_scene_get_int(buf);

	if (c3_scene_get_token(buf) != C3_SCENE_TOKEN_NUM)
		return (C3_PARSE_FAIL);
	height = c3_scene_get_int(buf);

	scene->resolution.x = width;
	scene->resolution.y = height;
	return (C3_PARSE_SUCCESS);
}

t_c3_parse_result	c3_scene_parse_texture(
	t_c3_scene *scene, t_c3_object_type typ, t_c3_scene_buffer *buf)
{
	return (0);
}

t_c3_parse_result	c3_scene_parse(t_c3_scene *scene, const char *path)
{
	return (0);
}

void	c3_scene_cleanup(t_c3_scene *scene)
{
	return ;
}