#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/01 10:28:15 by thisai            #+#    #+#              #
#    Updated: 2020/11/01 18:59:31 by thisai           ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = libft.a
SO = libft.so

CFLAGS = -Wall -Wextra -Werror
ARFLAGS = rcs

SRCS = ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c		\
	ft_memchr.c ft_memcmp.c ft_strlen.c ft_isalpha.c ft_isdigit.c		\
	ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c	\
	ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strlcpy.c ft_strlcat.c		\
	ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c			\
	ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c			\
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

HEADERS = libft.h

OBJS = $(SRCS:.c=.o)

.PHONY: so all bonus clean

all: $(NAME)

so: $(SO)

$(SO): $(OBJS)
	$(CC) -shared -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

bonus:

clean:
	$(RM) $(OBJS) *~

fclean: clean
	$(RM) $(NAME)

re: fclean all
