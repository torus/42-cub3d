/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 15:52:31 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 15:52:31 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "scene_int.h"

int			c3_scene_read_int(t_c3_scene_parser *buf)
{
	int		result;
	char	ch;

	ch = buf->getc(buf->container);
	result = 0;
	if (ch == '0')
		return (result);
	while (ch >= '0' && ch <= '9' && result < 100000)
	{
		result = result * 10 + (ch - '0');
		ch = buf->getc(buf->container);
	}
	if (ch >= 0)
		buf->ungetc(buf->container);
	return (result);
}

t_c3_token	c3_scene_get_token_expect(
	t_c3_scene_parser *buf, char next_char, t_c3_token token)
{
	int	ch;

	ch = buf->getc(buf->container);
	if (ch == next_char)
		return (token);
	buf->ungetc(buf->container);
	return (C3_SCENE_TOKEN_UNKNOWN);
}

t_c3_token	c3_scene_get_token_s(t_c3_scene_parser *buf)
{
	int	ch;

	ch = buf->getc(buf->container);
	if (ch == 'O')
		return (C3_SCENE_TOKEN_SO);
	buf->ungetc(buf->container);
	if (ch == ' ')
		return (C3_SCENE_TOKEN_S);
	return (C3_SCENE_TOKEN_UNKNOWN);
}

t_c3_token	c3_scene_get_token_head(t_c3_scene_parser *buf, int ch)
{
	buf->is_beginning_of_line = 0;
	if (ch < 0)
		return (C3_SCENE_TOKEN_EOF);
	if (ch == 'R')
		return (C3_SCENE_TOKEN_R);
	if (ch == 'N')
		return (c3_scene_get_token_expect(buf, 'O', C3_SCENE_TOKEN_NO));
	if (ch == 'S')
		return (c3_scene_get_token_s(buf));
	if (ch == 'W')
		return (c3_scene_get_token_expect(buf, 'E', C3_SCENE_TOKEN_WE));
	if (ch == 'E')
		return (c3_scene_get_token_expect(buf, 'A', C3_SCENE_TOKEN_EA));
	if (ch == 'F')
		return (C3_SCENE_TOKEN_F);
	if (ch == 'C')
		return (C3_SCENE_TOKEN_C);
	if (ch >= 0)
		buf->ungetc(buf->container);
	else
		return (C3_SCENE_TOKEN_EOF);
	return (C3_SCENE_TOKEN_POSSIBLY_MAP);
}

t_c3_token	c3_scene_get_token(t_c3_scene_parser *buf)
{
	int	ch;

	ch = buf->getc(buf->container);
	while (ch == '\n')
	{
		buf->is_beginning_of_line = 1;
		ch = buf->getc(buf->container);
	}
	if (buf->is_beginning_of_line)
		return (c3_scene_get_token_head(buf, ch));
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
	if (ch < 0)
		return (C3_SCENE_TOKEN_EOF);
	return (C3_SCENE_TOKEN_UNKNOWN);
}
