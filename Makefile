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

SRCS = cub3d.c utils.c
OBJS = $(SRCS:%.c=%.o)

NAME = cub3d

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -Wall -Wextra -Werror
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd

run-test:
	make -C test/

run: $(NAME)
	./$(NAME)

$(NAME)	:$(OBJS)
	$(CC) -o $(NAME) $^ $(LDFLAGS)
