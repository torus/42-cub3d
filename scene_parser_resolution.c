/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_resolution.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:17:36 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 16:17:36 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "scene_int.h"

t_c3_parse_result	c3_scene_parse_resolution(
	t_c3_scene *scene, t_c3_scene_parser *buf)
{
	int			width;
	int			height;

	if (c3_scene_get_token(buf) == C3_SCENE_TOKEN_NUM)
	{
		width = c3_scene_get_int(buf);
		if (c3_scene_get_token(buf) == C3_SCENE_TOKEN_NUM)
		{
			height = c3_scene_get_int(buf);
			scene->resolution.x = width;
			scene->resolution.y = height;
			return (C3_PARSE_SUCCESS);
		}
	}
	buf->error = "Invalid resolution format";
	return (C3_PARSE_FAIL);
}

int					c3_scene_try_resolution(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result)
{
	if (tok == C3_SCENE_TOKEN_R)
	{
		if (buf->is_specified[C3_SCENE_TOKEN_R])
		{
			buf->error = "R specified multiple times";
			*result = C3_PARSE_FAIL;
			return (0);
		}
		buf->is_specified[C3_SCENE_TOKEN_R] = 1;
		*result = c3_scene_parse_resolution(scene, buf);
		if (*result != C3_PARSE_SUCCESS)
			return (0);
	}
	return (1);
}
