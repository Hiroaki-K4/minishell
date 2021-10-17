#Makefile
NAME = minishell

.PHONY: all, clean, fclean, re

SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror

INCLUDE	= -I$(brew --prefix readline)/include

LDFLAGS = -L$(brew --prefix readline)/lib -lreadline -lhistory

all: $(NAME)

c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:.c=.o)

$(NAME):$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all
