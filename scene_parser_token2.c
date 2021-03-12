/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_token2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 15:59:58 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 15:59:58 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "scene_int.h"

int			c3_scene_get_int(t_c3_scene_parser *buf)
{
	return (buf->int_value);
}

const char	*c3_scene_get_string(t_c3_scene_parser *buf)
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

const char	*c3_scene_get_rest(t_c3_scene_parser *buf)
{
	char	ch;
	int		index;

	ch = buf->getc(buf->container);
	index = 0;
	while (ch > 0 && ch != '\n' && index < C3_STRING_BUFFER_SIZE - 1)
	{
		buf->string_value[index++] = ch;
		ch = buf->getc(buf->container);
	}
	if (ch >= 0)
		buf->ungetc(buf->container);
	buf->string_value[index] = '\0';
	return (buf->string_value);
}

t_c3_token	c3_scene_get_token_for_map(t_c3_scene_parser *buf)
{
	int	ch;

	ch = buf->getc(buf->container);
	if (ch == '\n')
	{
		buf->is_beginning_of_line = 1;
		ch = buf->getc(buf->container);
	}
	if (buf->is_beginning_of_line)
		return (c3_scene_get_token_head(buf, ch));
	if (ch < 0)
		return (C3_SCENE_TOKEN_EOF);
	return (C3_SCENE_TOKEN_UNKNOWN);
}
