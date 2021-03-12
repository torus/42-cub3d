/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_map.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 17:47:55 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 17:47:55 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <libft.h>
#include "scene.h"
#include "scene_int.h"

void				c3_map_rows_cleanup(t_c3_map_rows *rows)
{
	if (rows)
	{
		free(rows->row);
		c3_map_rows_cleanup(rows->next);
		free(rows);
	}
}

t_c3_map_rows		*t_c3_map_rows_create(
	t_c3_scene_parser *buf, t_c3_map_rows *rows, const char *str)
{
	t_c3_map_rows	*new_row;

	new_row = malloc(sizeof(t_c3_map_rows));
	if (!new_row)
	{
		buf->error = strerror(errno);
		c3_map_rows_cleanup(rows);
		return (NULL);
	}
	new_row->next = rows;
	new_row->row = ft_strdup(str);
	return (new_row);
}

t_c3_map_rows		*c3_scene_parse_read_rows(
	t_c3_scene *scene, t_c3_scene_parser *buf)
{
	t_c3_token		tok;
	t_c3_map_rows	*rows;
	int				width;

	rows = NULL;
	tok = C3_SCENE_TOKEN_POSSIBLY_MAP;
	while (tok == C3_SCENE_TOKEN_POSSIBLY_MAP)
	{
		if (!(rows = t_c3_map_rows_create(buf, rows, c3_scene_get_rest(buf))))
			return (NULL);
		width = ft_strlen(rows->row);
		tok = c3_scene_get_token_for_map(buf);
		scene->map_width = scene->map_width < width ? width : scene->map_width;
		scene->map_height++;
	}
	if (tok != C3_SCENE_TOKEN_EOF)
	{
		buf->error = "Invalid map format";
		c3_map_rows_cleanup(rows);
		return (NULL);
	}
	return (rows);
}

void				c3_scene_populate_map(
	t_c3_scene *scene, t_c3_map_rows *rows)
{
	t_c3_map_rows	*new_row;
	int				num_rows;
	int				width;
	char			*dest;

	num_rows = scene->map_height;
	while (num_rows > 0)
	{
		dest = &scene->map[(num_rows - 1) * scene->map_width];
		width = ft_strlen(rows->row);
		ft_memcpy(dest, rows->row, width);
		ft_memset(dest + width, ' ', scene->map_width - width);
		free(rows->row);
		new_row = rows->next;
		free(rows);
		rows = new_row;
		num_rows--;
	}
}

t_c3_parse_result	c3_scene_parse_map(
	t_c3_scene *scene, t_c3_scene_parser *buf)
{
	t_c3_map_rows	*rows;

	if (!(rows = c3_scene_parse_read_rows(scene, buf)))
		return (C3_PARSE_FAIL);
	if (!(scene->map = malloc(scene->map_width * scene->map_height)))
	{
		buf->error = strerror(errno);
		return (C3_PARSE_FAIL);
	}
	c3_scene_populate_map(scene, rows);
	return (C3_PARSE_SUCCESS);
}
