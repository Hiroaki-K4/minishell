#include "minishell.h"

int	g_pipes[2];
int	g_last_pid;

void	do_piping(int pipes[2], t_node *node)
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
		if (dup2(g_pipes[0], 0) < 0)
			exit_with_error("internal1 dup2 error");
		if (dup2(pipes[1], 1) < 0)
			exit_with_error("internal2 dup2 error");
	}
}

void	close_pipes(int pipes[2], t_node *node)
{
	if (pipes == NULL)
		return ;
	if (node->is_furthest_left)
		;
	else if (node->is_furthest_right)
		close(pipes[1]);
	else
		close(g_pipes[1]);
}

int	is_builtin_command(char *token, char **argv)
{
	int		ret;
	size_t	i;
	char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
	int (*builtin_funcs[])(char **) = {ft_echo, ft_cd, ft_pwd, ft_export, ft_unset, ft_env, ft_exit};

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(token, builtins[i], ft_strlen(builtins[i]) + 1))
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

int	execute_commands(t_node *node, char *envp[], int pipes[2])
{
	char	*path;
	char	**argv;
	int		status;
	pid_t	pid;

	argv = construct_argv(node->tokens);
	if (is_builtin_command(((t_token *)(node->tokens->content))->content, argv))
		return (SUCCESS);
	close_pipes(pipes, node);
	pid = fork();
	if (pid < 0)
		exit_with_error("fork error");
	else if (pid == 0)
	{
		do_piping(pipes, node);
		path = search(argv[0], envp);
		if (execve(path, argv, envp) == -1)
		{
			printf("minishell: %s: command not found\n", argv[0]);
			exit(errno);
		}
	}
	else
	{
		if (node->is_furthest_right)
		{
			if (waitpid(pid, &status, 0) < 0)
				exit_with_error("wait error");
		}
	}
	return (SUCCESS);
}

int	execute_pipe(t_node *ast, char *envp[])
{
	int		fildes[2];

	if (ast->lhs->attr == ND_PIPE)
	{
		execute_pipe(ast->lhs, envp);
		if (pipe(fildes) == FAIL)
			exit_with_error("pipe error");
		execute_commands(ast->lhs->rhs, envp, fildes);
		execute_commands(ast->rhs, envp, fildes);
		g_pipes[0] = fildes[0];
		g_pipes[1] = fildes[1];
	}
	else if (ast->lhs->attr == ND_COMMAND)
	{
		if (pipe(fildes) == FAIL)
			exit_with_error("pipe error");
		execute_commands(ast->lhs, envp, fildes);
		if (g_pipes[0] == 0 && g_pipes[1] == 0)
			execute_commands(ast->rhs, envp, fildes);
		g_pipes[0] = fildes[0];
		g_pipes[1] = fildes[1];
	}
	return (SUCCESS);
}

int	execute(t_node *ast, char *envp[])
{
	if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, envp));
	return (execute_commands(ast, envp, NULL));
}
