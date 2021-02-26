/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_texture.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:19:50 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 16:19:50 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include <libft.h>
#include "scene.h"
#include "scene_int.h"

t_c3_parse_result	c3_scene_parse_texture(
	t_c3_scene *scene, t_c3_object_type typ, t_c3_scene_parser *buf)
{
	const char	*path;

	path = c3_scene_get_string(buf);
	scene->tex_path[typ] = ft_strdup(path);
	if (!scene->tex_path[typ])
	{
		buf->error = strerror(errno);
		return (C3_PARSE_FAIL);
	}
	return (C3_PARSE_SUCCESS);
}

int					c3_scene_try_wall(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result)
{
	unsigned int		i;

	i = C3_SCENE_TOKEN_NO;
	while (i < C3_SCENE_TOKEN_NO + 5)
	{
		if (tok == i)
		{
			if (buf->is_specified[i])
			{
				buf->error = "some textures specified multiple times";
				*result = C3_PARSE_FAIL;
				return (0);
			}
			buf->is_specified[i] = 1;
			*result = c3_scene_parse_texture(
				scene, i - (C3_SCENE_TOKEN_NO - C3_OBJTYPE_WALL_N), buf);
			if (*result != C3_PARSE_SUCCESS)
				return (0);
		}
		i++;
	}
	return (1);
}

int					c3_scene_check_textures_specified(t_c3_scene_parser *buf)
{
	int	i;

	i = 0;
	while (i < C3_SCENE_TOKEN_NO + 5)
	{
		if (!buf->is_specified[i])
		{
			buf->error = "some textures are missing.";
			return (0);
		}
		i++;
	}
	return (1);
}
