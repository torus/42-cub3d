NAME = test

MLX_INC = ../../minilibx-linux
MLX_LIB = ../../minilibx-linux
LIBFT_DIR = ../../libft/libft

SRCS = parsecub.c ../scene.c ../bmp.c ../scene_parser_storage.c		\
	../scene_parser_token.c ../scene_parser_token2.c				\
	../scene_parser_color.c ../scene_parser_resolution.c			\
	../scene_parser_texture.c ../scene_parser_map.c ../loophook.c	\
	../ray.c ../ray2.c ../ray3.c ../minimap.c ../render2.c			\
	../render.c ../texture.c ../hooks.c ../map.c ../player.c		\
	../utils.c

OBJS = $(SRCS:%.c=%.o)
HEADERS = $(wildcard ../*.h)

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -I $(LIBFT_DIR) -Wall	\
	-Wextra -Werror -Wno-unused-parameter
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm	\
	-lbsd -L $(LIBFT_DIR) -lft

# test.bmp: bmptest
# 	./bmptest > $@

do-test: $(NAME)
	./$(NAME)

fail-test:
	for i in fail/*.cub; do \
		echo Testing $$i...; \
		../cub3d $$i && echo '[[=UNEXPECTEDLY SUCCESS=]]' || echo '[: SUCCESSFULLY FAILED :]'; \
	done

bmptest: bmptest.c ../bmp.c ../bmp.h
	$(CC) -o $@ $^

$(OBJS): $(HEADERS)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o $(NAME) *~ bmptest

.PHONY: do-test fail-test clean
