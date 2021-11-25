SRC_DIR := ./srcs
SRC = main.c lexer.c operate_list.c
SRCS := $(addprefix $(SRC_DIR)/,$(notdir $(SRC)))

OBJ_DIR := ./.objects
OBJS := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

INCLUDE = -I$(shell brew --prefix readline)/include
LDFLAGS = libft/libft.a -L$(shell brew --prefix readline)/lib -lreadline -lhistory

NAME = minishell

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror $(INCLUDE)

LIBFT = libft

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@-mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

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
