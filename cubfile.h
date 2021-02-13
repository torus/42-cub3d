/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubfile.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 14:14:05 by thisai            #+#    #+#             */
/*   Updated: 2021/02/08 14:14:05 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBFILE_H
# define CUBFILE_H

# include "cub3d.h"
# include "stddef.h"

typedef enum	e_c3_token
{
	C3_SCENE_TOKEN_R,			/* 0 */
	C3_SCENE_TOKEN_NO,
	C3_SCENE_TOKEN_SO,
	C3_SCENE_TOKEN_EA,
	C3_SCENE_TOKEN_WE,
	C3_SCENE_TOKEN_S,
	C3_SCENE_TOKEN_C,
	C3_SCENE_TOKEN_F,
	C3_SCENE_TOKEN_NUM,
	C3_SCENE_TOKEN_COMMA,
	C3_SCENE_TOKEN_POSSIBLY_MAP, /* 10 */
	C3_SCENE_TOKEN_EOF,
	C3_SCENE_TOKEN_UNKNOWN,
}		t_c3_token;

typedef enum	e_c3_parse_result
{
	C3_PARSE_FAIL,
	C3_PARSE_SUCCESS,
}		t_c3_parse_result;

typedef struct	s_c3_strbuf
{
	const char	*str;
	size_t		index;
}		t_c3_strbuf;

typedef struct	s_c3_file
{
	int		fd;
	int		is_ungotten;
	char	ungotten;
	char	last_char;
}		t_c3_file;

typedef union	u_c3_scene_container
{
	t_c3_strbuf	*strbuf;
	t_c3_file	*file;
}		t_c3_scene_container;

#define C3_STRING_BUFFER_SIZE 1024

typedef struct	s_c3_scene_parser
{
	t_c3_scene_container	container;
	int						is_beginning_of_line;
	int						int_value;
	char					string_value[C3_STRING_BUFFER_SIZE];
	int						(*getc)(t_c3_scene_container);
	void					(*ungetc)(t_c3_scene_container);
}		t_c3_scene_parser;


typedef struct	s_c3_scene
{
	t_c3_vector	resolution;
	char		*tex_path[C3_OBJTYPE_NUM];
	unsigned int	color_floor;
	unsigned int	color_ceiling;

	int			map_width;
	int			map_height;
	char		*map;
}		t_c3_scene;

typedef struct	s_c3_map_rows
{
	char	*row;
	struct	s_c3_map_rows	*next;
}		t_c3_map_rows;

t_c3_parse_result	c3_scene_parse_resolution(
	t_c3_scene *scene, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_texture(
	t_c3_scene *scene, t_c3_object_type typ, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_floor(
	t_c3_scene *scene, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_ceiling(
	t_c3_scene *scene, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_map(
	t_c3_scene *scene, t_c3_scene_parser *buf);

t_c3_parse_result	c3_scene_parse(t_c3_scene *scene, t_c3_scene_parser *buf);

void	c3_scene_init(t_c3_scene *scene);
void	c3_scene_cleanup(t_c3_scene *scene);


t_c3_token	c3_scene_get_token(t_c3_scene_parser *buf);
int			c3_scene_get_int(t_c3_scene_parser *buf);
const char*	c3_scene_get_string(t_c3_scene_parser *buf);
const char*	c3_scene_get_rest_of_line(t_c3_scene_parser *buf);
int			c3_scene_parser_init_with_file(
	t_c3_scene_parser *buf, const char *path);

#endif
