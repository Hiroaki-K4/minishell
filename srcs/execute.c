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

char	**construct_argv(t_list *tokens)
{
	size_t	idx;
	char	**argv;

	idx = 0;
	argv = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
	if (argv == NULL)
		return (NULL);
	while (tokens)
	{
		argv[idx] = (((t_token *)(tokens->content))->content);
		tokens = tokens->next;
		idx++;
	}
	argv[idx] = NULL;
	return (argv);
}

void	execute_command(char **argv, char *envp[])
{
	char	*path;

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
	argv = construct_argv(node->tokens);
	close_pipes(pipes, node, state);
	pid = fork();
	state->process_count++;
	if (pid < 0)
		exit_with_error("fork error");
	else if (pid == 0)
	{
		do_piping(pipes, node, state);
		execute_command(argv, envp);
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
