NAME = test

MLX_INC = ../../minilibx-linux
MLX_LIB = ../../minilibx-linux
LIBFT_DIR = ../../libft/libft

SRCS = parsecub.c ../scene.c ../utils.c ../scene_parser_storage.c	\
	../scene_parser_token.c ../scene_parser_token2.c				\
	../scene_parser_color.c ../scene_parser_resolution.c			\
	../scene_parser_texture.c ../scene_parser_map.c

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

bmptest: bmptest.c ../bmp.c ../bmp.h
	$(CC) -o $@ $^

$(OBJS): $(HEADERS)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
