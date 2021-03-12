/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:30:44 by thisai            #+#    #+#             */
/*   Updated: 2021/02/22 12:30:44 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_INT_H
# define CUB3D_INT_H

# include <stdint.h>
# include "cub3d.h"
# include "scene.h"

void		c3_bmp_put_int16(void *dest, int16_t val);
void		c3_bmp_put_int32(void *dest, int32_t val);
void		c3_bmp_generate(t_c3_state *stat);

void		c3_update(t_c3_state *stat);
void		c3_terminate(t_c3_state *stat);

void		c3_draw(t_c3_state *stat);
void		c3_render_scene(t_c3_state *stat);
char		c3_query_map(t_c3_state *stat, int x, int y);
void		c3_draw_map(t_c3_state *stat);
void		c3_draw_rays_on_map(t_c3_state *stat);
void		c3_scan(t_c3_state *stat);
uint32_t	c3_sample_texture(
	t_c3_state *stat, t_c3_object_type type, double u, double v);
void		c3_texture_preload_all(t_c3_state *stat);

void		c3_init_hooks(t_c3_state *stat);
int			c3_loop_hook(void *param);
int			c3_focusin_hook(void *param);
int			c3_focusout_hook(void *param);
int			c3_client_hook(void *param);
void		c3_check(int64_t val, const char *message);
int			c3_key_press_hook(int key, void *param);
int			c3_key_release_hook(int key, void *param);

void		c3_map_init(t_c3_map *map, t_c3_scene *scene);
char		c3_query_map(t_c3_state *stat, int x, int y);
void		c3_check_map_closed(t_c3_state *stat, int x, int y);
void		c3_check_map_closed_iter(t_c3_state *stat, int x, int y);

void		c3_renderer_cleanup(t_c3_renderer *rend);
void		c3_renderer_init(
	t_c3_renderer *rend, t_c3_scene *scene,
	int minimap_width, int minimap_height);
void		c3_init_render_target(t_c3_state *stat);
void		c3_init_set_screen_size(t_c3_state *stat, t_c3_scene *scene);
void		c3_start_rendering_loop(t_c3_state *stat);

void		c3_player_init(
	t_c3_state *stat, t_c3_player *player, t_c3_map *map);

double		c3_dot(t_c3_vector *origin, t_c3_vector *a, t_c3_vector *b);
double		c3_distance_squared(t_c3_vector *p1, t_c3_vector *p2);

int			c3_get_horizontal_hit(
	t_c3_state *stat, t_c3_vector *pos,
	double theta, t_c3_hit_result *result);
int			c3_get_vertical_hit(
	t_c3_state *stat, t_c3_vector *pos,
	double theta, t_c3_hit_result *result);
t_c3_vector	c3_get_horizontal_hit_for_index(
	double theta, int index, t_c3_vector *pos);
t_c3_vector	c3_get_horizontal_hit_cell_for_index(
	double theta, t_c3_vector *hit);
t_c3_vector	c3_get_vertical_hit_for_index(
	double theta, int index, t_c3_vector *pos);
t_c3_vector	c3_get_vertical_hit_cell_for_index(
	double theta, t_c3_vector *hit);
int			c3_get_horizontal_hit(
	t_c3_state *stat, t_c3_vector *pos,
	double theta, t_c3_hit_result *result);
int			c3_get_vertical_hit(
	t_c3_state *stat, t_c3_vector *pos,
	double theta, t_c3_hit_result *result);

#endif
