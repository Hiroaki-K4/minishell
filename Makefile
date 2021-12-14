SRC_DIR := ./srcs
SRC := main.c \
	   preprocess.c \
	   tokenize.c \
	   parse.c \
	   execute.c \
	   check_syntax.c \
	   check_char.c \
	   operate_list.c \
	   signal_handler.c
SRCS := $(addprefix $(SRC_DIR)/,$(notdir $(SRC)))

OBJ_DIR := ./.objects
OBJS := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

DEPS := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.d)))

INCLUDE := -I./includes -I$(shell brew --prefix readline)/include
LDFLAGS := libft/libft.a -L$(shell brew --prefix readline)/lib -lreadline -lhistory

NAME := minishell

CC := gcc
CFLAGS := -g -Wall -Wextra -Werror -MMD -MP

LIBFT := libft

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@-mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	@make clean -C $(LIBFT)
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) libft/libft.a
	$(RM) $(NAME)

re: fclean all

test: all
	@bash test.sh

-include $(DEPS)

.PHONY: all clean fclean re test
