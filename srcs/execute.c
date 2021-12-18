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
		if (!ft_strncmp(token, builtins[i], ft_strlen(builtins[i])))
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
	argv = (char **)malloc(sizeof(char *) * 1);
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

int	execute_command(t_node *ast, char *envp[])
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
			exit(errno);
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
	(void)ast;
	(void)envp;
	return (SUCCESS);
}

int	execute_semicolon(t_node *ast, char *envp[])
{
	(void)ast;
	(void)envp;
	return (SUCCESS);
}

int	execute(t_node *ast, char *envp[])
{
	if (ast->attr == ND_SEMICOLON)
		return (execute_semicolon(ast, envp));
	else if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, envp));
	return (execute_command(ast, envp));
}
