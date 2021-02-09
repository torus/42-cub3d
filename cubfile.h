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
	C3_SCENE_TOKEN_UNKNOWN,
	C3_SCENE_TOKEN_R,
	C3_SCENE_TOKEN_NO,
	C3_SCENE_TOKEN_SO,
	C3_SCENE_TOKEN_EA,
	C3_SCENE_TOKEN_WE,
	C3_SCENE_TOKEN_S,
	C3_SCENE_TOKEN_C,
	C3_SCENE_TOKEN_F,
	C3_SCENE_TOKEN_NUM,
	C3_SCENE_TOKEN_COMMA,
	C3_SCENE_TOKEN_POSSIBLY_MAP,
}		t_c3_token;

typedef struct	s_c3_strbuf
{
	const char	*str;
	size_t		index;
}		t_c3_strbuf;

typedef struct	s_c3_file
{
	int	fd;
}		t_c3_file;

typedef union	u_c3_scene_container
{
	t_c3_strbuf	*strbuf;
	t_c3_file	*file;
}		t_c3_scene_container;

#define C3_STRING_BUFFER_SIZE 1024

typedef struct	s_c3_scene_buffer
{
	t_c3_scene_container	container;
	int						is_beginning_of_line;
	int						int_value;
	char					string_value[C3_STRING_BUFFER_SIZE];
	int						(*getc)(t_c3_scene_container);
	void					(*ungetc)(t_c3_scene_container);
}		t_c3_scene_buffer;

int		c3_scene_parse_resolution_str(t_c3_scene *scene, const char *str);
int		c3_scene_parse_no_str(t_c3_scene *scene, const char *str);
int		c3_scene_parse(t_c3_scene *scene, const char *path);
void	c3_scene_cleanup(t_c3_scene *scene);


t_c3_token	c3_scene_get_token(t_c3_scene_buffer *buf);
int			c3_scene_get_int(t_c3_scene_buffer *buf);
const char*	c3_scene_get_string(t_c3_scene_buffer *buf);


#endif
