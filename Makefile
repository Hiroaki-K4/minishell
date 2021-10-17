#Makefile
NAME = minishell

.PHONY: all, clean, fclean, re

SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):$(OBJS)
		$(CC) $(CFLAGS) -I$(shell brew --prefix readline)/include -o $(NAME) $(OBJS) -L$(shell brew --prefix readline)/lib -lreadline -lhistory

clean:
	rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all
