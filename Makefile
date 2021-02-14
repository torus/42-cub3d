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

SRCS = cub3d.c utils.c scene.c
OBJS = $(SRCS:%.c=%.o)

NAME = cub3d

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -I $(LIBFT_DIR) -Wall -Wextra -Werror
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd -L $(LIBFT_DIR) -lft

# run-test:
# 	make -C test/

run: $(NAME)
	./$(NAME) test/sample.cub

$(NAME)	:$(OBJS)
	$(CC) -o $(NAME) $^ $(LDFLAGS)
