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

	return (-1);
}

int			c3_scene_get_int(t_c3_scene_buffer *buf)
{
	return (buf->int_value);
}

const char*	c3_scene_get_string(t_c3_scene_buffer *buf)
{
	return "ahoaho";
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

int		c3_scene_parse_resolution(t_c3_scene *scene, t_c3_scene_buffer *buf)
{
	return (-1);
}

int		c3_scene_parse_resolution_str(t_c3_scene *scene, const char *str)
{
/* 	(void)scene; */
/* 	t_c3_scene_buffer	buf; */
/* 	t_c3_strbuf			strbuf; */

/* 	strbuf.str = str; */
/* 	strbuf.index = 0; */
/* 	buf.getc = c3_strbuf_getc; */
/* 	buf.container.strbuf = &strbuf; */
/* 	buf.is_beginning_of_line = 1; */

/* 	int	id; */

/* 	id = c3_scene_parse_identifier(&buf); */
/* 	if (id == C3_SCENE_PARSE_ERROR) */
/* 		return (id); */
/* 	if (id != C3_SCENE_ID_R) */
/* 	{ */

/* 		return (C3_SCENE_PARSE_FAIL); */
/* 	} */

	return (0);
}

int		c3_scene_parse_no_str(t_c3_scene *scene, const char *str)
{
	return (0);
}

int		c3_scene_parse(t_c3_scene *scene, const char *path)
{
	return (0);
}

void	c3_scene_cleanup(t_c3_scene *scene)
{
	return ;
}
