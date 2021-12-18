#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
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
	TK_WORD = 0,
	TK_IO_NUMBER,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_REDIRECT_MULTI,
	TK_REDIRECT_APPEND,
	TK_SINGLE_QUOTED = 8,
	TK_DOUBLE_QUOTED,
	TK_SEMICOLON,
}	t_token_kind;

typedef enum e_node_kind
{
	ND_COMMAND,
	ND_SEMICOLON,
	ND_PIPE,
}	t_node_kind;

typedef struct s_token
{
	char				*content;
	t_token_kind		attr;
}	t_token;

typedef struct s_node
{
	t_node_kind		attr;
	struct s_node	*lhs;
	struct s_node	*rhs;
	t_list			*tokens;
}	t_node;

void			output_result(void *content);

void			init_sigaction(struct sigaction *si, struct sigaction *sq);
void			sigint_handler(int sig);
void			sigquit_handler(int sig);

int				is_metacharacter(char c);
int				is_quotes(char c);
int				is_separating_character(char c);

t_token			*make_token(char *line, size_t pos, size_t len, t_token_kind a);
int				ft_lstadd_node(t_list **token_list, t_token *new_token);

t_node			*preprocess(char *input);

t_token_kind	decide_attr(char *line, int pos, size_t *i);
void			tokenize(char *line, t_list **token_list);

int				check_syntax(t_list *token_list);

t_node			*parse(t_list **token_list);

int				execute(t_node *ast, char *envp[]);

char			*search(char *path, char **envp);

#endif
