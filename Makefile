VPATH := srcs/:srcs/builtins/:srcs/execute/:srcs/parse/:srcs/tokenize/:srcs/expand/

SRC_DIR := ./srcs
SRC := main.c \
	   preprocess.c \
	   check_syntax.c \
	   check_char.c \
	   operate_list.c \
	   operate_envs.c \
	   signal_handler.c \
	   error.c \
	   builtins/is_builtin.c \
	   builtins/echo.c \
	   builtins/cd.c \
	   builtins/pwd.c \
	   builtins/export.c \
	   builtins/print_envs.c \
	   builtins/unset.c \
	   builtins/env.c \
	   builtins/exit.c \
	   execute/execute.c \
	   execute/construct.c \
	   execute/search.c \
	   execute/pipe.c \
	   execute/redirect.c \
	   parse/parse.c \
	   parse/parse_utils.c \
	   tokenize/tokenize.c \
	   tokenize/separate.c \
	   tokenize/get_token_kind.c \
	   expand/expand.c
SRCS := $(notdir $(SRC))

OBJ_DIR := ./.objects
OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

DEPS := $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.d))

INCLUDE := -I./includes -I$(shell brew --prefix readline)/include
LDFLAGS := libft/libft.a -L$(shell brew --prefix readline)/lib -lreadline -lhistory

NAME := minishell

CC := gcc
CFLAGS := -g -Wall -Wextra -Werror -MMD -MP

LIBFT := libft

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@-mkdir -p $(OBJ_DIR) $(DEP_DIR)
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

dot: all
	@dot -Tsvg ast.dot > ast.svg

-include $(DEPS)

.PHONY: all clean fclean re test dot
