#Makefile
SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

INCLUDE	= -I$(shell brew --prefix readline)/include

LDFLAGS = -L$(shell brew --prefix readline)/lib -lreadline -lhistory

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror $(INCLUDE)

all: $(NAME)

c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME):$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re