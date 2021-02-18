NAME = test
SRCS = parsecub.c ../scene.c ../utils.c
OBJS = $(SRCS:%.c=%.o)
HEADERS = $(wildcard ../*.h)

MLX_INC = ../../minilibx-linux
MLX_LIB = ../../minilibx-linux
LIBFT_DIR = ../../libft/libft

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -I $(LIBFT_DIR) -Wall -Wextra -Werror -Wno-unused-parameter
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd -L $(LIBFT_DIR) -lft

# test.bmp: bmptest
# 	./bmptest > $@

do-test: $(NAME)
	./$(NAME)

bmptest: bmptest.c ../bmp.c ../bmp.h
	$(CC) -o $@ $^

$(OBJS): $(HEADERS)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
