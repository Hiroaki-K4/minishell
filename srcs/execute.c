#include "minishell.h"

int	g_last_pid;

void	do_piping(int pipes[2], int is_asynchronous)
{
	if (pipes == NULL)
		return ;
	if (is_asynchronous)
	{
		if (dup2(pipes[1], 1) < 0)
			exit(errno);
		close(pipes[0]);
	}
	else
	{
		if (dup2(pipes[0], 0) < 0)
			exit(errno);
	}
}

void	close_pipes(int pipes[2])
{
	if (pipes == NULL)
		return ;
	if (pipes[0] >= 0)
		close(pipes[0]);
	if (pipes[1] >= 0)
		close(pipes[1]);
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

int	execute_commands(t_node *ast, char *envp[], int pipes[2], int is_asynchronous)
{
	char	*path;
	char	**argv;
	int		status;
	pid_t	pid;

	argv = construct_argv(ast->tokens);
	if (is_builtin_command(((t_token *)(ast->tokens->content))->content, argv))
		return (SUCCESS);
	if (pipes && !is_asynchronous)
		close(pipes[1]);
	pid = fork();
	if (pid < 0)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		do_piping(pipes, is_asynchronous);
		path = search(argv[0], envp);
		if (execve(path, argv, envp) == -1)
		{
			printf("minishell: %s: command not found\n", argv[0]);
			exit(errno);
		}
	}
	else
	{
		if (!is_asynchronous)
		{
			if (waitpid(pid, &status, 0) < 0)
			{
				printf("Error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return (SUCCESS);
}

// int	execute_command_internal(t_node *command, char *envp[], int pipe[2])
// {

// }

int	execute_pipe(t_node *ast, char *envp[], int pipes[2])
{
	int		fildes[2];

	(void)pipes;
	if (pipe(fildes) == FAIL)
		exit(errno);
	if (ast->lhs->attr == ND_PIPE)
		execute_pipe(ast->lhs, envp, fildes);
	else if (ast->lhs->attr == ND_COMMAND)
	{
		execute_commands(ast->lhs, envp, fildes, TRUE);
		execute_commands(ast->rhs, envp, fildes, FALSE);
	}
	return (SUCCESS);
}

int	execute(t_node *ast, char *envp[])
{
	if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, envp, NULL));
	return (execute_commands(ast, envp, NULL, FALSE));
}
