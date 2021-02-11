NAME = test
SRCS = $(wildcard *.c) ../cubfile.c ../utils.c
OBJS = $(SRCS:%.c=%.o)

MLX_INC = ../../minilibx-linux
MLX_LIB = ../../minilibx-linux
LIBFT_DIR = ../../libft/libft

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -I $(LIBFT_DIR) -Wall -Wextra -Werror -Wno-unused-parameter
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd -L $(LIBFT_DIR) -lft

do-test: $(NAME)
	./$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
