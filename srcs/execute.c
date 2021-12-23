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

int	is_builtin_command(char **argv)
{
	int		ret;
	size_t	i;
	char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
	int (*builtin_funcs[])(char **) = {ft_echo, ft_cd, ft_pwd, ft_export, ft_unset, ft_env, ft_exit};

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(argv[0], builtins[i], ft_strlen(builtins[i]) + 1))
		{
			ret = builtin_funcs[i](argv);
			(void)ret;  // TODO: use ret value
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

void	set_redirect(t_list **tokens, t_global_state *state)
{
	char	*input;
	char	*tmp_fp;

	if (*tokens == NULL)
		return ;
	if (((t_token *)((*tokens)->content))->attr == TK_IO_NUMBER)
	{
		state->redirect_fd = ft_atoi(((t_token *)((*tokens)->content))->content);
		*tokens = (*tokens)->next;
	}
	if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_OUT)
	{
		*tokens = (*tokens)->next;
		state->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (state->file_fd < 0)
			exit_with_error("failed to open");
		if (state->redirect_fd == -1)
			state->redirect_fd = 1;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_IN)
	{
		*tokens = (*tokens)->next;
		state->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDONLY);
		if (state->file_fd < 0)
			exit_with_error("failed to open");
		if (state->redirect_fd == -1)
			state->redirect_fd = 0;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_DGREAT)
	{
		*tokens = (*tokens)->next;
		state->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (state->file_fd < 0)
			exit_with_error("failed to open");
		if (state->redirect_fd == -1)
			state->redirect_fd = 1;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_DLESS)
	{
		*tokens = (*tokens)->next;
		state->here_delimiter = ((t_token *)((*tokens)->content))->content;
		input = readline("> ");
		state->here_document = input;
		while (ft_strncmp(input, state->here_delimiter, ft_strlen(state->here_delimiter) + 1))
		{
			input = readline("> ");
			if (!ft_strncmp(input, state->here_delimiter, ft_strlen(state->here_delimiter) + 1))
				break ;
			state->here_document = ft_strjoin(state->here_document, ft_strjoin(ft_strdup("\n"), input));
		}
		state->here_document = ft_strjoin(state->here_document, ft_strdup("\n"));
		tmp_fp = ft_strjoin(getenv("PWD"), "/minishell_tmp");
		state->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
		write(state->file_fd, state->here_document, ft_strlen(state->here_document));
		close(state->file_fd);
		state->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
		unlink(tmp_fp);
		if (state->redirect_fd == -1)
			state->redirect_fd = 0;
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

	idx = 0;
	argv = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
	if (argv == NULL)
		return (NULL);
	while (TRUE)
	{
		if (tokens == NULL)
			break ;
		while (TRUE)
		{
			if (tokens == NULL || is_redirect_token((t_token *)(tokens->content)))
				break ;
			argv[idx] = (((t_token *)(tokens->content))->content);
			tokens = tokens->next;
			idx++;
		}
		set_redirect(&tokens, state);
	}
	argv[idx] = NULL;
	return (argv);
}

void	execute_command(char **argv, char *envp[], t_global_state *state)
{
	char	*path;

	if (state->file_fd)
		dup2(state->file_fd, state->redirect_fd);
	if (is_builtin_command(argv))
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
	int		count;
	char	**argv;
	int		status;
	pid_t	pid;

	count = 0;
	argv = construct_argv(node->tokens, state);
	close_pipes(pipes, node, state);
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
		if (node->is_furthest_right)
		{
			while (count < state->process_count)
			{
				if (state->pids[count] && waitpid(state->pids[count], &status, 0) < 0)
					exit_with_error("wait error");
				count++;
			}
			if (state->file_fd)
				close(state->file_fd);
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
