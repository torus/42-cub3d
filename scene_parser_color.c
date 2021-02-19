/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_color.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:11:32 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 16:11:32 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "scene_int.h"

static t_c3_parse_result	parse_color(
	unsigned int *result, t_c3_scene_parser *buf)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	if (c3_scene_get_token(buf) == C3_SCENE_TOKEN_NUM)
	{
		r = c3_scene_get_int(buf);
		if (c3_scene_get_token(buf) == C3_SCENE_TOKEN_COMMA
			&& c3_scene_get_token(buf) == C3_SCENE_TOKEN_NUM)
		{
			g = c3_scene_get_int(buf);
			if (c3_scene_get_token(buf) == C3_SCENE_TOKEN_COMMA
				&& c3_scene_get_token(buf) == C3_SCENE_TOKEN_NUM)
			{
				b = c3_scene_get_int(buf);
				*result = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
				return (C3_PARSE_SUCCESS);
			}
		}
	}
	buf->error = "Invalid color format";
	return (C3_PARSE_FAIL);
}

t_c3_parse_result			c3_scene_parse_floor(
	t_c3_scene *scene, t_c3_scene_parser *buf)
{
	return (parse_color(&scene->color_floor, buf));
}

t_c3_parse_result			c3_scene_parse_ceiling(
	t_c3_scene *scene, t_c3_scene_parser *buf)
{
	return (parse_color(&scene->color_ceiling, buf));
}

int							c3_scene_try_ceiling(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result)
{
	if (tok == C3_SCENE_TOKEN_C)
	{
		if (buf->is_specified[C3_SCENE_TOKEN_C])
		{
			buf->error = "C specified multiple times";
			*result = C3_PARSE_FAIL;
			return (0);
		}
		buf->is_specified[C3_SCENE_TOKEN_C] = 1;
		*result = c3_scene_parse_ceiling(scene, buf);
		if (*result != C3_PARSE_SUCCESS)
			return (0);
	}
	return (1);
}

int							c3_scene_try_floor(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result)
{
	if (tok == C3_SCENE_TOKEN_F)
	{
		if (buf->is_specified[C3_SCENE_TOKEN_F])
		{
			buf->error = "F specified multiple times";
			*result = C3_PARSE_FAIL;
			return (0);
		}
		buf->is_specified[C3_SCENE_TOKEN_F] = 1;
		*result = c3_scene_parse_floor(scene, buf);
		if (*result != C3_PARSE_SUCCESS)
			return (0);
	}
	return (1);
}
