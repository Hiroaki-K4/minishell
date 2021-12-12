#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define TRUE 1
# define FALSE 0
# define SUCCESS 0
# define FAIL -1

typedef enum e_token_kind
{
	TK_WORD,
	TK_SPACE,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_REDIRECT_MULTI,
	TK_REDIRECT_APPEND,
	TK_PARENTHESIS,
	TK_SINGLE_QUOTED,
	TK_DOUBLE_QUOTED,
	TK_SEMICOLON
}	t_token_kind;

typedef struct s_command
{
	int					attr;
	char				*content;
	struct s_command	*next;
}	t_command;

int				preprocess(char *input);
int				check_syntax(t_list *token_list);
int				is_metacharacter(char c);
int				is_quotes(char c);
int				is_separating_character(char c);
int				ft_lstadd_node(t_list **token_list, t_command *new_token);
void			tokenize(char *line, t_list **token_list);
void			sigint_handler(int sig);
void			sigquit_handler(int sig);
t_token_kind	decide_attr(char *line, int pos);
t_command		*make_token(t_command *token, char *line, size_t pos, int i);

#endif
