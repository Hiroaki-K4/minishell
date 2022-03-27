VPATH := srcs/:srcs/builtins/:srcs/execute/:srcs/parse/:srcs/tokenize/:srcs/expand/:srcs/signal

SRC_DIR := ./srcs
SRC := main.c \
	   preprocess.c \
	   check_syntax.c \
	   check_char.c \
	   operate_list.c \
	   operate_envs.c \
	   error.c \
	   ft_lstadd.c \
	   get_next_line.c \
	   utils.c \
	   builtins/is_builtin.c \
	   builtins/echo.c \
	   builtins/cd.c \
	   builtins/cd_utils.c \
	   builtins/cd_utils2.c \
	   builtins/pwd.c \
	   builtins/export.c \
	   builtins/print_envs.c \
	   builtins/set_env.c \
	   builtins/unset.c \
	   builtins/env.c \
	   builtins/exit.c \
	   execute/execute.c \
	   execute/execute_pipe.c \
	   execute/execute_commands.c \
	   execute/wait_all_processes.c \
	   execute/construct.c \
	   execute/search.c \
	   execute/pipe.c \
	   execute/redirect.c \
	   execute/read_heredocument.c \
	   parse/parse.c \
	   parse/parse_utils.c \
	   tokenize/tokenize.c \
	   tokenize/separate.c \
	   tokenize/get_token_kind.c \
	   expand/expand.c \
	   expand/expand_env_vals.c \
	   expand/expand_env_vals_core.c \
	   expand/check_quote.c \
	   expand/remove_quote.c \
	   expand/make_e_state.c \
	   expand/free_and_return_status.c \
	   expand/expand_word_in_heredocument.c \
	   expand/get_env_name_here_ver.c \
	   signal/signal_handler.c \
	   signal/set_handlers.c
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
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) libft/libft.a
	$(RM) $(NAME)

re: fclean all

test: all
	@bash test.sh 2> /dev/null

debug: CFLAGS += -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: re

dot: all
	@dot -Tsvg ast.dot > ast.svg

-include $(DEPS)

.PHONY: all clean fclean re test dot
