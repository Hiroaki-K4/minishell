#include "minishell.h"

void	do_piping(int pipes[2], t_node *node, t_global_state *state)
{
	if (pipes == NULL)
		return ;
	if (node->is_furthest_left)
	{
		if (dup2(pipes[1], 1) < 0)
			exit_with_error("furthest left dup2 error");
		close(pipes[0]);
	}
	else if (node->is_furthest_right)
	{
		if (dup2(pipes[0], 0) < 0)
			exit_with_error("furthest right dup2 error");
	}
	else
	{
		if (dup2(state->old_pipes[0], 0) < 0)
			exit_with_error("internal1 dup2 error");
		if (dup2(pipes[1], 1) < 0)
			exit_with_error("internal2 dup2 error");
	}
}

void	close_pipes(int pipes[2], t_node *node, t_global_state *state)
{
	if (pipes == NULL)
		return ;
	if (node->is_furthest_left)
		;
	else if (node->is_furthest_right)
		close(pipes[1]);
	else
		close(state->old_pipes[1]);
}

int	is_special_builtin_command(char **argv, t_env **envs)
{
	int		ret;
	size_t	i;
	char *builtins[] = {"cd", "export", "unset", "exit", NULL};
	int (*builtin_funcs[])(char **, t_env **) = {ft_cd, ft_export, ft_unset, ft_exit};

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(argv[0], builtins[i], ft_strlen(builtins[i]) + 1))
		{
			ret = builtin_funcs[i](argv, envs);
			(void)ret;  // TODO: use ret value
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

int	is_builtin_command(char **argv, t_env *envs)
{
	int		ret;
	size_t	i;
	char *builtins[] = {"echo", "pwd", "env", NULL};
	int (*builtin_funcs[])(char **, t_env *) = {ft_echo, ft_pwd, ft_env};

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(argv[0], builtins[i], ft_strlen(builtins[i]) + 1))
		{
			ret = builtin_funcs[i](argv, envs);
			(void)ret;  // TODO: use ret value
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

void	init_redirect(t_redirect *redirect)
{
	redirect->redirect_fd = -1;
	redirect->file_fd = -1;
	redirect->here_delimiter = NULL;
	redirect->here_document = NULL;
}

void	set_redirect(t_list **tokens, t_redirect *redirect, t_env *envs)
{
	char	*input;
	char	*tmp_fp;

	if (*tokens == NULL)
		return ;
	if (((t_token *)((*tokens)->content))->attr == TK_IO_NUMBER)
	{
		redirect->redirect_fd = ft_atoi(((t_token *)((*tokens)->content))->content);
		*tokens = (*tokens)->next;
	}
	if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_OUT)
	{
		*tokens = (*tokens)->next;
		redirect->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (redirect->file_fd < 0)
			exit_with_error(((t_token *)((*tokens)->content))->content);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 1;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_IN)
	{
		*tokens = (*tokens)->next;
		redirect->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDONLY);
		if (redirect->file_fd < 0)
			exit_with_error(((t_token *)((*tokens)->content))->content);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 0;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_DGREAT)
	{
		*tokens = (*tokens)->next;
		redirect->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (redirect->file_fd < 0)
			exit_with_error(((t_token *)((*tokens)->content))->content);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 1;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_DLESS)
	{
		*tokens = (*tokens)->next;
		redirect->here_delimiter = ((t_token *)((*tokens)->content))->content;
		input = readline("> ");
		redirect->here_document = input;
		while (ft_strncmp(input, redirect->here_delimiter, ft_strlen(redirect->here_delimiter) + 1))
		{
			input = readline("> ");
			if (!ft_strncmp(input, redirect->here_delimiter, ft_strlen(redirect->here_delimiter) + 1))
				break ;
			redirect->here_document = ft_strjoin(redirect->here_document, ft_strjoin(ft_strdup("\n"), input));
		}
		redirect->here_document = ft_strjoin(redirect->here_document, ft_strdup("\n"));
		tmp_fp = ft_strjoin(get_env("PWD", envs), "/minishell_tmp");
		redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
		write(redirect->file_fd, redirect->here_document, ft_strlen(redirect->here_document));
		close(redirect->file_fd);
		redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
		unlink(tmp_fp);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 0;
	}
	*tokens = (*tokens)->next;
}

int	is_redirect_token(t_token *token)
{
	return (token->attr == TK_IO_NUMBER
		|| token->attr == TK_REDIRECT_IN
		|| token->attr == TK_REDIRECT_OUT
		|| token->attr == TK_REDIRECT_DGREAT
		|| token->attr == TK_REDIRECT_DLESS);
}

char	**construct_argv(t_list *tokens, t_global_state *state)
{
	size_t	idx;
	char	**argv;
	t_redirect	*tmp;

	idx = 0;
	argv = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
	if (argv == NULL)
		return (NULL);
	while (TRUE)
	{
		while (TRUE)
		{
			if (tokens == NULL || is_redirect_token((t_token *)(tokens->content)))
				break ;
			argv[idx] = (((t_token *)(tokens->content))->content);
			tokens = tokens->next;
			idx++;
		}
		if (tokens == NULL)
			break ;
		state->redirect_num++;
		tmp = (t_redirect *)malloc(sizeof(t_redirect) * state->redirect_num);
		if (tmp == NULL)
			return (NULL);
		int i = 0;
		while (i < state->redirect_num - 1)
		{
			tmp[i] = state->redirects[i];
			i++;
		}
		state->redirects = tmp;
		init_redirect(&state->redirects[state->redirect_num - 1]);
		set_redirect(&tokens, &state->redirects[state->redirect_num - 1], state->envs);
	}
	argv[idx] = NULL;
	return (argv);
}

void	execute_command(char **argv, char *envp[], t_global_state *state)
{
	char	*path;

	int i = 0;
	while (i < state->redirect_num)
	{
		if (state->redirects[i].file_fd)
			dup2(state->redirects[i].file_fd, state->redirects[i].redirect_fd);
		i++;
	}
	if (is_builtin_command(argv, state->envs))
		exit(errno);
	else
	{
		path = search(argv[0], envp);
		if (execve(path, argv, envp) == -1)
		{
			printf("minishell: %s: command not found\n", argv[0]);
			exit(errno);
		}
	}
}

int	execute_commands(t_node *node, char *envp[], int pipes[2], t_global_state *state)
{
	char	**argv;
	int		status;
	pid_t	pid;

	argv = construct_argv(node->tokens, state);
	close_pipes(pipes, node, state);
	if (is_special_builtin_command(argv, &(state->envs)))
		return (SUCCESS);
	state->process_count++;
	pid = fork();
	if (pid < 0)
		exit_with_error("fork error");
	else if (pid == 0)
	{
		do_piping(pipes, node, state);
		execute_command(argv, envp, state);
	}
	else
	{
		state->pids[state->process_count - 1] = pid;
		int i = 0;
		while (i < state->redirect_num)
		{
			if (state->redirects[i].file_fd != -1)
				close(state->redirects[i].file_fd);
			i++;
		}
		if (node->is_furthest_right)
		{
			while (state->process_count)
			{
				if (waitpid(-1, &status, 0) < 0)
					exit_with_error("wait error");
				state->process_count--;
			}
		}
	}
	return (SUCCESS);
}

int	execute_pipe(t_node *ast, char *envp[], t_global_state *state)
{
	int		fildes[2];

	if (ast->lhs->attr == ND_PIPE)
	{
		execute_pipe(ast->lhs, envp, state);
		if (pipe(fildes) == FAIL)
			exit_with_error("pipe error");
		execute_commands(ast->lhs->rhs, envp, fildes, state);
		if (ast->is_top)
		{
			state->old_pipes[0] = fildes[0];
			state->old_pipes[1] = fildes[1];
			execute_commands(ast->rhs, envp, fildes, state);
		}
		state->old_pipes[0] = fildes[0];
		state->old_pipes[1] = fildes[1];
	}
	else if (ast->lhs->attr == ND_COMMAND)
	{
		if (pipe(fildes) == FAIL)
			exit_with_error("pipe error");
		execute_commands(ast->lhs, envp, fildes, state);
		if (ast->is_top)
			execute_commands(ast->rhs, envp, fildes, state);
		state->old_pipes[0] = fildes[0];
		state->old_pipes[1] = fildes[1];
	}
	return (SUCCESS);
}

int	execute(t_node *ast, char *envp[], t_global_state *state)
{
	if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, envp, state));
	return (execute_commands(ast, envp, NULL, state));
}
