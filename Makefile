#Makefile
SRCS = srcs/main.c srcs/preprocess.c srcs/operate_list.c srcs/store_token.c

OBJS = $(SRCS:.c=.o)

INCLUDE	= -I$(shell brew --prefix readline)/include

LDFLAGS = libft/libft.a -L$(shell brew --prefix readline)/lib -lreadline -lhistory

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror $(INCLUDE)

LIBFT = libft

all: $(NAME)

c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME):$(OBJS)
		make -C $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	make clean -C $(LIBFT)
	rm -f $(OBJS)

fclean: clean
		rm -f libft/libft.a
		rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re