/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsecub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 14:21:11 by thisai            #+#    #+#             */
/*   Updated: 2021/02/13 19:56:09 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "../cub3d.h"
#include "../scene.h"
#include "../scene_int.h"

int		success_count = 0;
int		fail_count = 0;

void	check(int64_t val, const char *msg)
{
	if (val)
	{
		success_count++;
		printf("✔ %s\n", msg);
	}
	else
	{
		fail_count++;
		printf("✖ %s\n", msg);
	}
}

void	print_result()
{
	printf("✔: %d ✖: %d\n", success_count, fail_count);
}

#define CHECK(val) check((int64_t)val, #val)

void	set_strbuf(t_c3_scene_parser *buf, t_c3_strbuf *strbuf, const char *str)
{
	strbuf->str = str;
	strbuf->index = 0;
	buf->getc = c3_strbuf_getc;
	buf->ungetc = c3_strbuf_ungetc;
	buf->container.strbuf = strbuf;
	buf->is_beginning_of_line = 1;

	int i = 0;
	while (i < C3_SCENE_TOKEN_NUM)
		buf->is_specified[i++] = 0;
}

int main()
{
	t_c3_scene_parser	buf;
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
	const char *str = c3_scene_get_rest(&buf);
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
	CHECK(!strcmp(buf.error, "Invalid resolution format"));

	set_strbuf(&buf, &strbuf, " ./path_to_the_north_texture");
	buf.is_beginning_of_line = 0;
	c3_scene_parse_texture(&scene, C3_OBJTYPE_WALL_N, &buf);
	CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_N],
				  "./path_to_the_north_texture"));

	set_strbuf(&buf, &strbuf, " ./path_to_the_south_texture");
	buf.is_beginning_of_line = 0;
	c3_scene_parse_texture(&scene, C3_OBJTYPE_WALL_S, &buf);
	CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_S],
				  "./path_to_the_south_texture"));

	set_strbuf(&buf, &strbuf, " 220,100,0");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_floor(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.color_floor == 0x00dc6400);

	set_strbuf(&buf, &strbuf, " 100,200,255");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_ceiling(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.color_ceiling == 0x0064c8ff);

	set_strbuf(&buf, &strbuf, " 100,300,255");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_ceiling(&scene, &buf) == C3_PARSE_FAIL);

	set_strbuf(&buf, &strbuf, " 100,300,255aa");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_ceiling(&scene, &buf) == C3_PARSE_FAIL);

	set_strbuf(&buf, &strbuf, " 100,300,255,");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_ceiling(&scene, &buf) == C3_PARSE_FAIL);

	set_strbuf(
		&buf, &strbuf,
		"        1001000000000000000000001\n"
		"111111111011000001110000000000001\n"
		"100000000011000001110111111111111\n"
		"11110111111111011100000010001\n");
	buf.is_beginning_of_line = 0;
	CHECK(c3_scene_parse_map(&scene, &buf) == C3_PARSE_SUCCESS);
	CHECK(scene.map_width == 33);
	CHECK(scene.map_height == 4);
	CHECK(scene.map[33 * 1 + 9] == '0');

	c3_scene_cleanup(&scene);

	{
		t_c3_scene	scene;
		c3_scene_init(&scene);




		set_strbuf(
			&buf, &strbuf,
			"R 1920 1080\n"
			"NO ./path_to_the_north_texture\n"
			"SO ./path_to_the_south_texture\n"
			"WE ./path_to_the_west_texture\n"
			"EA ./path_to_the_east_texture\n"
			"\n"
			"S ./path_to_the_sprite_texture\n"
			"F 220,100,0\n"
			"C 225,30,0\n"
			"\n"
			"        1111111111111111111111111\n"
			"        1000000000110000000000001\n"
			"        1011000001110000002000001\n"
			"        1001000000000000000000001\n"
			"111111111011000001110000000000001\n"
			"100000000011000001110111111111111\n"
			"11110111111111011100000010001\n"
			"11110111111111011101010010001\n"
			"11000000110101011100000010001\n"
			"10002000000000001100000010001\n"
			"10000000000000001101010010001\n"
			"11000001110101011111011110N0111\n"
			"11110111 1110101 101111010001\n"
			"11111111 1111111 111111111111\n"
			);

		CHECK(c3_scene_parse(&scene, &buf) == C3_PARSE_SUCCESS);

		CHECK(scene.resolution.x == 1920);
		CHECK(scene.resolution.y == 1080);
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_N],
					  "./path_to_the_north_texture"));
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_S],
					  "./path_to_the_south_texture"));
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_E],
					  "./path_to_the_east_texture"));
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_W],
					  "./path_to_the_west_texture"));
		CHECK(scene.color_floor == 0x00dc6400);
		CHECK(scene.color_ceiling == 0x00e11e00);

		CHECK(scene.map[33 *  0 +  0] == ' ');
		CHECK(scene.map[33 *  2 + 26] == '2');
		CHECK(scene.map[33 *  1 +  0] == ' ');
		CHECK(scene.map[33 *  2 +  0] == ' ');
		CHECK(scene.map[33 *  3 +  0] == ' ');
		CHECK(scene.map[33 *  3 +  8] == '1');
		CHECK(scene.map[33 * 11 + 26] == 'N');
		CHECK(scene.map[33 * 12 +  8] == ' ');
		CHECK(scene.map[33 * 13 + 28] == '1');

		c3_scene_cleanup(&scene);
	}

	{
		t_c3_scene	scene;
		c3_scene_init(&scene);

		set_strbuf(
			&buf, &strbuf,
			"R 1920 1080\n"
			"NO ./path_to_the_north_texture\n"
			"SO ./path_to_the_south_texture\n"
			"WE ./path_to_the_west_texture\n"
			"EA ./path_to_the_east_texture\n"
			"S ./path_to_the_sprite_texture\n"
			"F 220,100,0\n"
			"11111\n"
			"10001\n"
			"10S01\n"
			"10001\n"
			"11111\n"
			);

		CHECK(c3_scene_parse(&scene, &buf) == C3_PARSE_SUCCESS);
		CHECK(!c3_scene_check_colors_specified(&buf));
		c3_scene_cleanup(&scene);
	}

	{
		t_c3_scene	scene;
		c3_scene_init(&scene);

		set_strbuf(
			&buf, &strbuf,
			"R 1920 1080\n"
			"NO ./path_to_the_north_texture\n"
			"SO ./path_to_the_south_texture\n"
			"WE ./path_to_the_west_texture\n"
			"EA ./path_to_the_east_texture\n"
			"S ./path_to_the_sprite_texture\n"
			"C 225,30,0\n"
			"11111\n"
			"10001\n"
			"10S01\n"
			"10001\n"
			"11111\n"
			);

		CHECK(c3_scene_parse(&scene, &buf) == C3_PARSE_SUCCESS);
		CHECK(!c3_scene_check_colors_specified(&buf));
		c3_scene_cleanup(&scene);
	}

	{
		t_c3_scene	scene;
		c3_scene_init(&scene);

		set_strbuf(
			&buf, &strbuf,
			"R 1920 1080\n"
			"SO ./path_to_the_south_texture\n"
			"WE ./path_to_the_west_texture\n"
			"EA ./path_to_the_east_texture\n"
			"S ./path_to_the_sprite_texture\n"
			"F 220,100,0\n"
			"C 225,30,0\n"
			"11111\n"
			"10001\n"
			"10S01\n"
			"10001\n"
			"11111\n"
			);

		CHECK(c3_scene_parse(&scene, &buf) == C3_PARSE_SUCCESS);
		CHECK(!c3_scene_check_textures_specified(&buf));
		c3_scene_cleanup(&scene);
	}

	{
		t_c3_scene_parser	buf;
		t_c3_file			file;

		buf.container.file = &file;
		CHECK(c3_scene_parser_init_with_file(&buf, "fail/sample.cub"));

		t_c3_scene	scene;
		c3_scene_init(&scene);
		CHECK(c3_scene_parse(&scene, &buf) == C3_PARSE_SUCCESS);

		CHECK(scene.resolution.x == 1920);
		CHECK(scene.resolution.y == 1080);
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_N],
					  "./path_to_the_north_texture"));
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_S],
					  "./path_to_the_south_texture"));
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_E],
					  "./path_to_the_east_texture"));
		CHECK(!strcmp(scene.tex_path[C3_OBJTYPE_WALL_W],
					  "./path_to_the_west_texture"));
		CHECK(scene.color_floor == 0x00dc6400);
		CHECK(scene.color_ceiling == 0x00e11e00);

		CHECK(scene.map[33 *  0 +  0] == ' ');
		CHECK(scene.map[33 *  2 + 26] == '2');
		CHECK(scene.map[33 *  1 +  0] == ' ');
		CHECK(scene.map[33 *  2 +  0] == ' ');
		CHECK(scene.map[33 *  3 +  0] == ' ');
		CHECK(scene.map[33 *  3 +  8] == '1');
		CHECK(scene.map[33 * 11 + 26] == 'N');
		CHECK(scene.map[33 * 12 +  8] == ' ');
		CHECK(scene.map[33 * 13 + 28] == '1');

		c3_scene_cleanup(&scene);
	}

	print_result();
	if (fail_count)
		return (1);
	return (0);
}
