#Makefile
NAME = minishell

.PHONY: all, clean, fclean, re

SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):$(OBJS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline -L$(shell brew --prefix readline)/lib -I$(shell brew --prefix readline)/include

clean:
	rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all
