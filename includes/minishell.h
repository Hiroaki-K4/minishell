#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
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

char **g_envs;

typedef enum e_token_kind
{
	TK_WORD = 0,
	TK_IO_NUMBER,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_REDIRECT_DLESS,
	TK_REDIRECT_DGREAT,
	TK_SINGLE_QUOTED = 8,
	TK_DOUBLE_QUOTED,
	TK_SEMICOLON,
}	t_token_kind;

typedef enum e_node_kind
{
	ND_COMMAND,
	ND_PIPE,
	ND_SEMICOLON,
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
	int				is_furthest_left;
	int				is_furthest_right;
	int				is_top;
}	t_node;

typedef struct s_redirect
{
	int		redirect_fd;
	int		file_fd;
	char	*here_delimiter;
	char	*here_document;
}	t_redirect;

typedef struct s_global_state
{
	int			old_pipes[2];
	int			process_count;
	int			*pids;
	int			last_command_exit_status;
	t_redirect	*redirects;
	int			redirect_num;
}	t_global_state;

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

int				expand(t_list *token_list, t_list **expanded_list);

t_node			*parse(t_list **token_list);

int				execute(t_node *ast, char *envp[], t_global_state *state);

char			*search(char *path, char **envp);

int				ft_echo(char **argv);
int				ft_cd(char **argv);
int				ft_pwd(char **argv);
int				ft_export(char **argv);
int				ft_unset(char **argv);
int				ft_env(char **argv);
int				ft_exit(char **argv);

void			exit_with_error(char *msg);

int				print_envs();
int				get_env_pos(char *env_name);
int				get_first_char_pos(char *word, char c);


int				init_envs(char **envp);
char			**sort_envs(char **dup_env);
char			*get_env(char *env);

#endif
