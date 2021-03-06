/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:46:05 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/29 00:21:52 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
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

# define BUFFER_SIZE 10000
# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif
# define REDIRECT_PROMPT "> "

typedef enum e_token_kind
{
	TK_WORD = 0,
	TK_IO_NUMBER,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_REDIRECT_DLESS,
	TK_REDIRECT_DGREAT,
	TK_SEMICOLON,
}	t_token_kind;

typedef enum e_node_kind
{
	ND_COMMAND,
	ND_PIPE,
	ND_SEMICOLON,
}	t_node_kind;

typedef enum e_quote_state
{
	NORMAL,
	IN_QUOTE,
	IN_DQUOTE,
}	t_quote_state;

typedef struct s_tokenize_state
{
	size_t			trim_start;
	size_t			current_pos;
	t_quote_state	quote_state;
	t_token_kind	token_kind;
}	t_tokenize_state;

typedef struct s_token
{
	char			*content;
	t_token_kind	attr;
	int				q_removed;
}	t_token;

typedef struct s_expand_state
{
	size_t			start;
	size_t			current_pos;
	t_quote_state	quote_state;
	t_token			*original_token;
	t_list			*token_list;
}	t_expand_state;

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
	int		q_removed;
	char	*here_delimiter;
	char	*here_document;
}	t_redirect;

typedef struct s_envs
{
	int		envs_num;
	char	**content;
}	t_envs;

typedef struct s_global_state
{
	int					old_pipes[2];
	int					process_count;
	int					max_process_num;
	size_t				max_redirect_num;
	pid_t				*pids;
	int					last_command_exit_status;
	t_redirect			**redirects;
	size_t				redirect_num;
	t_envs				*envs;
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;
}	t_global_state;

void			output_result(void *content);
int				get_next_line(int fd, char **line);

void			set_sigaction(struct sigaction *sa, void (*handler)(int));
void			sigint_handler(int sig);
void			sigint_handler2(int sig);
void			set_initial_handlers(t_global_state *state);
void			set_parent_handlers(t_global_state *state);
void			set_child_handlers(t_global_state *state);
void			set_redirect_handlers(t_global_state *state);

int				is_metacharacter_with_token_kind(char c);
int				is_metacharacter_without_token_kind(char c);
int				is_separating_word(char *line, int pos);
int				is_quote(char c);
int				is_redirect_char(char c);

t_token			*make_token(char *line, size_t pos, size_t len, t_token_kind a);
void			ft_lstdelone_all(t_list *lst, void (*del)(void*));
void			ft_lstclear_all(t_list **lst, void (*del)(void*));

int				ft_lstadd_token(t_list **token_list, t_token *new_token);
int				ft_lstadd_word(t_list **lst, char *new_word);
int				ft_lstadd_last(t_list **lst, t_list *new);

void			do_piping(int pipes[2], t_node *node, t_global_state *state);
void			close_pipes(int pipes[2], t_node *node, t_global_state *state);
void			close_parent_pipe(int pipes[2], t_node *n, t_global_state *s);

int				read_heredocument(t_redirect *redirect, char *content,
					t_envs *envs, int exit_status);
int				set_redirect(t_list **tokens, t_redirect *redirect,
					t_envs *envs, int exit_status);

t_node			*preprocess(char *input, t_global_state *state, int debug);

t_token_kind	get_token_kind(char *line, int pos, size_t *i);
t_token_kind	get_token_kind_about_quote(t_quote_state state, char c);

void			update_quote_state(t_quote_state *state, char c);
int				tokenize(char *line, t_list **token_list);

int				separate_by_no_kind_sep_word(char *line, t_list **token_list,
					t_tokenize_state *tokenize_state);
int				separate_by_sep_word(char *line, t_list **token_list,
					t_tokenize_state *tokenize_state);
int				separate_by_null_char(char *line, t_list **token_list,
					t_tokenize_state *tokenize_state);

int				check_syntax(t_list *token_list);

int				expand_env_vals(t_expand_state *e_state, t_envs *envs,
					int exit_status);

char			*get_env_name_here_ver(char *input, size_t *current_pos);
char			*check_name_here_ver(char *name, t_envs *envs, int exit_status);

char			*expand_word_in_heredocument(char *input, t_envs *envs,
					int exit_status);

t_expand_state	*fill_diff_between_start_curernt_pos(t_expand_state *e_state);
t_expand_state	*expand_env_vals_core(t_expand_state *e_state, t_envs *envs,
					int exit_status);

t_list			*check_quote(t_expand_state *e_state);

char			*get_word_in_quote(t_token *token, t_expand_state *e_state,
					char *word, int *q_removed);

t_expand_state	*make_e_state(t_list *token_list, t_envs *envs,
					int exit_status);

int				free_and_return_status(char *str, int status);

int				expand(t_list *token_lst, t_list **expanded_lst, t_envs *envs,
					int exit_status);
void			init_expand_state(t_expand_state *e_state);

t_node			*parse(t_list **token_list);
int				is_command_token(t_list **token_list);
t_node			*new_node(t_node *lhs, t_node *rhs, t_node_kind attr);
int				consume_token(t_list **token_list, t_token_kind kind);

void			wait_all_processes(t_global_state *state);
int				execute_commands(t_node *node, int pipes[2], t_global_state *s);
int				execute_pipe(t_node *ast, t_global_state *state);
int				execute(t_node *ast, t_global_state *state);
void			free_ast(t_node *ast);
char			**construct_argv(t_list *tokens, t_global_state *state);

char			*search(char *path, t_envs *envs);

int				is_special_builtin_command(char **argv, t_global_state *state);
int				is_builtin_command(char **argv, t_envs *envs, int *exit_status);

int				ft_echo(char **argv, t_envs *envs);
int				ft_pwd(char **argv, t_envs *envs);
int				ft_env(char **argv, t_envs *envs);
int				ft_cd(char **argv, t_envs **envs);
void			free_strings(char **argv);
void			set_curpath(char **argv, t_envs **envs, char **curpath);
void			convert_curpath_to_canonical_form(char **curpath);
int				ft_export(char **argv, t_envs **envs);
void			free_double_word(char *word1, char *word2);
int				ft_unset(char **argv, t_envs **envs);
int				ft_exit(char **argv, t_envs **envs, int last_exit_status);

void			exit_with_error(char *msg);
void			print_command_error(char *command, char *msg);
void			print_error(char *msg);

int				print_envs(t_envs *envs);
int				get_env_pos(char *env_name, t_envs *envs);
int				get_first_char_pos(char *word, char c);

int				set_env(char *name, char *val, t_envs **envs);

int				init_envs(t_envs **envs, char **envp);
char			**sort_envs(char **dup_env);
int				get_env_name_and_value(char **name, char **value, char *env);
char			*get_env(char *name, t_envs *envs);
int				set_env(char *name, char *value, t_envs **envs);

void			*ft_realloc(void *ptr, size_t size);
void			**ft_realloc_array(void **ptr, size_t num);
void			realloc_redirects(t_global_state *state);

#endif
