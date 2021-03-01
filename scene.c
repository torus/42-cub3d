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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libft.h>

#include "cub3d.h"
#include "scene.h"
#include "scene_int.h"

t_c3_parse_result	c3_scene_parse(t_c3_scene *scene, t_c3_scene_parser *buf)
{
	t_c3_token			tok;
	t_c3_parse_result	result;

	tok = c3_scene_get_token(buf);
	while (tok != C3_SCENE_TOKEN_POSSIBLY_MAP)
	{
		if (tok == C3_SCENE_TOKEN_EOF)
		{
			buf->error = "Map not included";
			return (C3_PARSE_FAIL);
		}
		if (!c3_scene_try_resolution(scene, buf, tok, &result))
			return (result);
		if (!c3_scene_try_wall(scene, buf, tok, &result))
			return (result);
		if (!c3_scene_try_ceiling(scene, buf, tok, &result))
			return (result);
		if (!c3_scene_try_floor(scene, buf, tok, &result))
			return (result);
		tok = c3_scene_get_token(buf);
	}
	result = c3_scene_parse_map(scene, buf);
	return (result);
}

void				c3_scene_init(t_c3_scene *scene)
{
	int	i;

	scene->map_width = 0;
	scene->map_height = 0;
	i = 0;
	while (i < C3_OBJTYPE_NUM)
		scene->tex_path[i++] = NULL;
	scene->map = NULL;
}

void				c3_scene_cleanup(t_c3_scene *scene)
{
	int				i;

	i = 0;
	while (i < C3_OBJTYPE_NUM)
	{
		free(scene->tex_path[i]);
		scene->tex_path[i] = NULL;
		i++;
	}
	free(scene->map);
}

int					c3_scene_parser_init_with_file(
	t_c3_scene_parser *buf, const char *path)
{
	int	fd;
	int	i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		buf->error = strerror(errno);
		return (0);
	}
	buf->container.file->fd = fd;
	buf->container.file->is_ungotten = 0;
	buf->getc = c3_file_getc;
	buf->ungetc = c3_file_ungetc;
	buf->is_beginning_of_line = 1;
	buf->error = NULL;
	i = 0;
	while (i < C3_SCENE_TOKEN_NUM)
		buf->is_specified[i++] = 0;
	return (1);
}

int					c3_read_scene(t_c3_scene *scene, const char *path)
{
	t_c3_scene_parser	buf;
	t_c3_file			file;
	int					pathlen;

	c3_scene_init(scene);
	pathlen = ft_strlen(path);
	if (pathlen < 4 || ft_strncmp(path + pathlen - 4, ".cub", 4))
		C3_LOG("Error\n%s: Unknown file type.\n", path);
	else
	{
		buf.container.file = &file;
		if (!c3_scene_parser_init_with_file(&buf, path))
			C3_LOG("Error\n%s\n", buf.error);
		else
		{
			if (c3_scene_parse(scene, &buf) != C3_PARSE_SUCCESS
				|| !c3_scene_check_textures_specified(&buf)
				|| !c3_scene_check_colors_specified(&buf))
				C3_LOG("Error\n%s\n", buf.error);
			else
				return (1);
		}
	}
	return (0);
}
