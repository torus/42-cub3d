NAME = test
SRCS = $(wildcard *.c) ../cubfile.c
OBJS = $(SRCS:%.c=%.o)

MLX_INC = ../../minilibx-linux
MLX_LIB = ../../minilibx-linux

CFLAGS = -g -fsanitize=address -I $(MLX_INC) -Wall -Wextra -Wno-unused-parameter
LDFLAGS = -fsanitize=address -L $(MLX_LIB) -lmlx -lXext -lX11 -lm -lbsd

do-test: $(NAME)
	./$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
