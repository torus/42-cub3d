/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsecub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 14:21:11 by thisai            #+#    #+#             */
/*   Updated: 2021/02/08 14:21:11 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../cub3d.h"
#include "../cubfile.h"

void	check(int64_t val, const char *msg)
{
	if (val)
	{
		printf("✔ %s\n", msg);
	}
	else
	{
		printf("✖ %s\n", msg);
	}
}

#define CHECK(val) check((int64_t)val, #val)

int		c3_strbuf_getc(t_c3_scene_container cont);
void	c3_strbuf_ungetc(t_c3_scene_container cont);

void	set_strbuf(t_c3_scene_buffer *buf, t_c3_strbuf *strbuf, const char *str)
{
	strbuf->str = str;
	strbuf->index = 0;
	buf->getc = c3_strbuf_getc;
	buf->ungetc = c3_strbuf_ungetc;
	buf->container.strbuf = strbuf;
	buf->is_beginning_of_line = 1;
}

int main()
{
	t_c3_scene_buffer	buf;
	t_c3_strbuf			strbuf;

	set_strbuf(&buf, &strbuf, "R 1920 1080");
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_R);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 1920);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 1080);

	set_strbuf(&buf, &strbuf, "F 220,100,0");
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_F);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 220);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_COMMA);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 100);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_COMMA);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 0);


	set_strbuf(&buf, &strbuf, "EA ./path_to_the_east_texture");
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_EA);
	CHECK(!strcmp(c3_scene_get_string(&buf), "./path_to_the_east_texture"));

	set_strbuf(
		&buf, &strbuf,
		"R 1920 1080\n"
		"F 220,100,0\n"
		"EA ./path_to_the_east_texture");
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_R);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 1920);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 1080);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_F);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 220);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_COMMA);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 100);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_COMMA);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_NUM);
	CHECK(c3_scene_get_int(&buf) == 0);
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_EA);
	CHECK(!strcmp(c3_scene_get_string(&buf), "./path_to_the_east_texture"));

	set_strbuf(&buf, &strbuf, "        1000000000110000000000001");
	CHECK(c3_scene_get_token(&buf) == C3_SCENE_TOKEN_POSSIBLY_MAP);
	const char *str = c3_scene_get_rest_of_line(&buf);
	CHECK(!strcmp(str, "        1000000000110000000000001"));



	t_c3_scene	scene;
	c3_scene_init(&scene);

	set_strbuf(&buf, &strbuf, " 1920 1080");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_resolution(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.resolution.x == 1920);
	CHECK(scene.resolution.y == 1080);

	set_strbuf(&buf, &strbuf, " 192 108");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_resolution(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.resolution.x == 192);
	CHECK(scene.resolution.y == 108);

	set_strbuf(&buf, &strbuf, " 192");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_resolution(&scene, &buf) == C3_PARSE_FAIL);

	set_strbuf(&buf, &strbuf, " ./path_to_the_north_texture");
	buf.is_beginning_of_line = 0;
	c3_scene_parse_texture(&scene, C3_OBJTYPE_WALL_N, &buf);
	CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_N],
				  "./path_to_the_north_texture"));

	set_strbuf(&buf, &strbuf, " 220,100,0");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_floor(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.color_floor == 0x00dc6400);

	set_strbuf(&buf, &strbuf, " 100,200,300");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_ceiling(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.color_ceiling == 0x0064c82c);


	c3_scene_parse(&scene, "sample.cub");

	c3_scene_cleanup(&scene);
}
