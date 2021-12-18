#include "minishell.h"

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

int	execute_commands(t_node *ast, char *envp[])
{
	char	*path;
	char	**argv;
	int		status;
	pid_t	pid;

	argv = construct_argv(ast->tokens);
	if (is_builtin_command(((t_token *)(ast->tokens->content))->content, argv))
		return (SUCCESS);
	pid = fork();
	if (pid < 0)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		path = search(argv[0], envp);
		if (execve(path, argv, envp) == -1)
		{
			printf("minishell: %s: command not found\n", argv[0]);
			exit(errno);
		}
	}
	else if (waitpid(pid, &status, 0) < 0)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}

int	execute_pipe(t_node *ast, char *envp[])
{
	char	*path;
	char	**argv;
	int		status;
	pid_t	lhs_pid;
	pid_t	rhs_pid;
	int		fildes[2];

	if (ast->lhs->attr == ND_PIPE)
		execute_pipe(ast->lhs, envp);
	else if (ast->lhs->attr == ND_COMMAND)
	{
		if (pipe(fildes) == FAIL)
			exit(errno);
		argv = construct_argv(ast->lhs->tokens);
		if (is_builtin_command(((t_token *)(ast->lhs->tokens->content))->content, argv))
			return (SUCCESS);
		lhs_pid = fork();
		if (lhs_pid < 0)
		{
			printf("Error\n");
			exit(EXIT_FAILURE);
		}
		else if (lhs_pid == 0)
		{
			close(fildes[0]);
			dup2(fildes[1], 1);
			path = search(argv[0], envp);
			if (execve(path, argv, envp) == -1)
			{
				printf("minishell: %s: command not found\n", argv[0]);
				exit(errno);
			}
		}
		else
		{
			if (is_builtin_command(((t_token *)(ast->rhs->tokens->content))->content, argv))
				return (SUCCESS);
			close(fildes[1]);
			rhs_pid = fork();
			if (rhs_pid < 0)
			{
				printf("Error\n");
				exit(EXIT_FAILURE);
			}
			else if (rhs_pid == 0)
			{
				dup2(fildes[0], 0);
				argv = construct_argv(ast->rhs->tokens);
				path = search(argv[0], envp);
				if (execve(path, argv, envp) == -1)
				{
					printf("minishell: %s: command not found\n", argv[0]);
					exit(errno);
				}
			}
			close(fildes[0]);
			if (waitpid(rhs_pid, &status, 0) < 0)
			{
				printf("Error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return (SUCCESS);
}

int	execute(t_node *ast, char *envp[])
{
	if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, envp));
	return (execute_commands(ast, envp));
}
