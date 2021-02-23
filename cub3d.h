/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 14:15:50 by thisai            #+#    #+#             */
/*   Updated: 2021/02/08 14:15:50 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define C3_CHECK(val, mesg) c3_check((int64_t)val, mesg)
# define C3_MAX_COLLINEAR_SPRITES 10
# define C3_TRANSPARENT_COLOR 0x00ffffff

# define c3_log printf

typedef struct	s_c3_imgdata
{
	char	*data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}				t_c3_imgdata;

typedef struct	s_c3_keystate
{
	char	w;
	char	a;
	char	s;
	char	d;
	char	left;
	char	right;
}				t_c3_keystate;

typedef struct	s_c3_vector
{
	double	x;
	double	y;
}				t_c3_vector;

typedef struct	s_c3_player
{
	t_c3_vector	position;
	double		direction;
	double		walk_speed;
	double		rotation_speed;
}				t_c3_player;

typedef enum	e_c3_object_type
{
	C3_OBJTYPE_WALL_N,
	C3_OBJTYPE_WALL_S,
	C3_OBJTYPE_WALL_E,
	C3_OBJTYPE_WALL_W,
	C3_OBJTYPE_SPRITE,
	C3_OBJTYPE_NUM
}				t_c3_object_type;

typedef struct	s_c3_hit_result
{
	t_c3_object_type	type;
	t_c3_vector			position;
	double				offset;
	double				distance_sqared;
}				t_c3_hit_result;

typedef struct	s_c3_ray
{
	double			angle;
	t_c3_hit_result	hits[1 + C3_MAX_COLLINEAR_SPRITES];
	int				hit_sprite_count;
}				t_c3_ray;

typedef struct	s_c3_renderer
{
	double			plane_distance;
	double			fov;
	unsigned int	ceiling_color;
	unsigned int	floor_color;
	int				resolution_x;
	int				resolution_y;
	int				minimap_width;
	int				minimap_height;
	t_c3_ray		*rays;
}				t_c3_renderer;

typedef struct	s_c3_sprite
{
	t_c3_object_type	type;
	int					index;
}				t_c3_sprite;

typedef struct	s_c3_map
{
	char		*map;
	int			width;
	int			height;
}				t_c3_map;

#define C3_MAP_SYMBOL_EMPTY 0
#define C3_MAP_SYMBOL_WALL 1
#define C3_MAP_SYMBOL_SPRITE 2

typedef struct	s_c3_texture
{
	void	*image;
	char	*data;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}				t_c3_texture;

typedef struct	s_c3_texture_cache
{
	char			**path;
	t_c3_texture	cache[C3_OBJTYPE_NUM];
}				t_c3_texture_cache;

typedef struct	s_c3_state
{
	void				*mlx;
	void				*window;
	int					screen_width;
	int					screen_height;
	void				*img;
	t_c3_imgdata		imgdata;
	t_c3_keystate		keystate;
	t_c3_player			player;
	t_c3_renderer		renderer;
	t_c3_map			map;
	t_c3_texture_cache	*texture_cache;
	void				*scene;
	int					is_drawing_minimap;
	int					is_benchmarking;
	int					is_showing_screen;
}				t_c3_state;

typedef struct	s_bitmap_file_header
{
	unsigned char	file_type[2];
	unsigned char	file_size[4];
	unsigned char	reserved1[2];
	unsigned char	reserved2[2];
	unsigned char	offset[4];
}				t_bitmap_file_header;

typedef struct	s_bitmap_info_header
{
	unsigned char	header_size[4];
	unsigned char	image_width[4];
	unsigned char	image_height[4];
	unsigned char	planes[2];
	unsigned char	bpp[2];
	unsigned char	compression[4];
	unsigned char	image_size[4];
	unsigned char	resolution_x[4];
	unsigned char	resolution_y[4];
	unsigned char	num_colors[4];
	unsigned char	num_important_colors[4];
}				t_bitmap_info_header;

typedef struct	s_c3_ray_caster
{
	t_c3_hit_result	hori_hits[1 + C3_MAX_COLLINEAR_SPRITES];
	t_c3_hit_result	vert_hits[1 + C3_MAX_COLLINEAR_SPRITES];
	int				hori_sprites;
	int				vert_sprites;
	double			wall_distance;
}				t_c3_ray_caster;

#endif
