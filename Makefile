#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/13 16:22:39 by thisai            #+#    #+#              #
#    Updated: 2021/01/13 18:07:39 by thisai           ###   ########.fr        #
#                                                                              #
#******************************************************************************#

MLX_INC = ../minilibx-linux
MLX_LIB = ../minilibx-linux
LIBFT_DIR = ../libft/libft

SRCS = cub3d.c utils.c scene.c bmp.c scene_parser_storage.c			\
	scene_parser_token.c scene_parser_token2.c scene_parser_color.c	\
	scene_parser_resolution.c scene_parser_texture.c				\
	scene_parser_map.c

HEADERS = cub3d.h scene.h scene_int.h
OBJS = $(SRCS:%.c=%.o)

NAME = cub3d

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -I $(LIBFT_DIR) -Wall -Wextra -Werror
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd -L $(LIBFT_DIR) -lft

# run-test:
# 	make -C test/

run: $(NAME)
	./$(NAME) test/valid.cub --save

$(OBJS): $(HEADERS)

$(NAME)	:$(OBJS)
	$(CC) -o $(NAME) $^ $(LDFLAGS)
