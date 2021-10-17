#Makefile
NAME = minishell

.PHONY: all, clean, fclean, re

SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

INCLUDE	= -I. -I$(shell brew --prefix readline)/include

LDFLAGS = -L$(shell brew --prefix readline)/lib -lreadline -lhistory

CC = gcc

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

c.o: $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:.c=.o)
# c.o: gcc -I$(brew --prefix readline)/include -L$(brew --prefix readline)/lib -lreadline -lhistory srcs/main.c

$(NAME):$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all
