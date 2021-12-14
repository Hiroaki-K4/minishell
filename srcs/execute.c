#include "minishell.h"

int	is_builtin(char *command)
{
	if (!ft_strncmp(command, "echo", 5))
		return (TRUE);
	else if (!ft_strncmp(command, "exit", 5))
		exit(EXIT_SUCCESS);
	return (FALSE);
}

char	**reconstruct_argv(t_list *commands)
{
	char	**argv;

	argv = (char **)malloc(sizeof(char *) * 1);
	argv[0] = ((t_command *)(commands->content))->content;
	return (argv);
}

int	execute_command(t_node *ast, char *envp[])
{
	int		status;
	char	*path;
	char	**argv;
	pid_t	pid;

	argv = reconstruct_argv(ast->commands);
	if (is_builtin(((t_command *)(ast->commands->content))->content))
	{
		printf("builtin\n");
		return (SUCCESS);
	}
	pid = fork();
	if (pid < 0)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		path = ft_strjoin("/bin/",
				((t_command *)(ast->commands->content))->content);
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
