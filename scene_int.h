/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 15:45:56 by thisai            #+#    #+#             */
/*   Updated: 2021/02/19 15:45:56 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_INT_H
# define SCENE_INT_H

void				c3_strbuf_ungetc(t_c3_scene_container cont);
int					c3_strbuf_getc(t_c3_scene_container cont);
void				c3_file_ungetc(t_c3_scene_container cont);
int					c3_file_getc(t_c3_scene_container cont);

t_c3_token			c3_scene_get_token(t_c3_scene_parser *buf);
int					c3_scene_try_ceiling(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result);
int					c3_scene_try_floor(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result);
int					c3_scene_try_resolution(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result);
int					c3_scene_try_wall(
	t_c3_scene *scene, t_c3_scene_parser *buf,
	t_c3_token tok, t_c3_parse_result *result);
t_c3_parse_result	c3_scene_parse_map(
	t_c3_scene *scene, t_c3_scene_parser *buf);

int					c3_scene_get_int(t_c3_scene_parser *buf);
const char			*c3_scene_get_string(t_c3_scene_parser *buf);
const char			*c3_scene_get_rest(t_c3_scene_parser *buf);
t_c3_token			c3_scene_get_token_for_map(t_c3_scene_parser *buf);
t_c3_token			c3_scene_get_token_head(t_c3_scene_parser *buf, int ch);

t_c3_parse_result	c3_scene_parse_resolution(
	t_c3_scene *scene, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_texture(
	t_c3_scene *scene, t_c3_object_type typ, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_floor(
	t_c3_scene *scene, t_c3_scene_parser *buf);
t_c3_parse_result	c3_scene_parse_ceiling(
	t_c3_scene *scene, t_c3_scene_parser *buf);
void				c3_scene_cleanup(t_c3_scene *scene);

#endif
