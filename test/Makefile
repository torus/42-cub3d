NAME = test

MLX_INC = ../minilibx-linux
MLX_LIB = ../minilibx-linux
LIBFT_DIR = ../libft

SRCS = parsecub.c ../scene.c ../bmp.c ../scene_parser_storage.c			\
	../scene_parser_token.c ../scene_parser_token2.c					\
	../scene_parser_color.c ../scene_parser_color2.c					\
	../scene_parser_resolution.c ../scene_parser_texture.c				\
	../scene_parser_map.c ../loophook.c ../ray.c ../ray2.c ../ray3.c	\
	../minimap.c ../render2.c ../render.c ../texture.c ../hooks.c		\
	../hooks2.c ../map.c ../map2.c ../player.c ../utils.c

OBJS = $(SRCS:%.c=%.o)
HEADERS = $(wildcard ../*.h)

CFLAGS = -I $(MLX_INC) -I $(LIBFT_DIR) -Wall -Wextra -Werror	\
	-Wno-unused-parameter
LDFLAGS = -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd -L $(LIBFT_DIR)	\
	-lft

VALGRIND = valgrind --show-leak-kinds=all --leak-check=full -q

# test.bmp: bmptest
# 	./bmptest > $@

do-test: $(NAME)
	./$(NAME)

fail-test:
	@cd .. && \
		echo Testing invalid file name...; \
		$(VALGRIND) ./cub3D test/fail/invalid-filename.ceb \
			&& echo '[[=====UNEXPECTEDLY SUCCESS=====]]' \
			|| echo '[: SUCCESSFULLY FAILED :]'
	@cd .. && for i in test/fail/*.cub; do \
		echo Testing $$i...; \
		$(VALGRIND) ./cub3D $$i \
			&& echo '[[=====UNEXPECTEDLY SUCCESS=====]]' \
			|| echo '[: SUCCESSFULLY FAILED :]'; \
	done
	@cd .. && $(VALGRIND) ./cub3D test/valid.cub --sawe \
			&& echo '[[=====UNEXPECTEDLY SUCCESS=====]]' \
			|| echo '[: SUCCESSFULLY FAILED :]';
	@cd .. && $(VALGRIND) ./cub3D test/valid.cub --saved \
			&& echo '[[=====UNEXPECTEDLY SUCCESS=====]]' \
			|| echo '[: SUCCESSFULLY FAILED :]'; \

bmptest: bmptest.c ../bmp.c
	$(CC) -o $@ $^

$(OBJS): $(HEADERS)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o $(NAME) *~ bmptest

.PHONY: do-test fail-test clean
