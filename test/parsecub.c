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
	t_c3_scene	scene;
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









	CHECK(c3_scene_parse_resolution_str(&scene, "R 1920 1080"));
	CHECK(scene.resolution.x == 1920);
	CHECK(scene.resolution.y == 1080);

	CHECK(c3_scene_parse_resolution_str(&scene, "R 192 108"));
	CHECK(scene.resolution.x == 192);
	CHECK(scene.resolution.y == 108);

	CHECK(c3_scene_parse_resolution_str(&scene, "R 192") < 0);

	c3_scene_parse_no_str(&scene, "NO ./path_to_the_north_texture");


	c3_scene_parse(&scene, "sample.cub");

	c3_scene_cleanup(&scene);
}
