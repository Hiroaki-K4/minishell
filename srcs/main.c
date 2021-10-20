#include "minishell.h"

void	sigint_handler()
{
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

bool	check_builtin(char *command)
{
	if (!ft_strncmp(command, "echo", 5))
		return (true);
	else if (!ft_strncmp(command, "exit", 5))
		return (true);
	return (false);
}


void	run_command(char *command, char *envp[])
{
	pid_t	pid;
	int status;
	char *path;
	char *argv[] = {NULL, NULL};

	argv[0] = command;
	if (check_builtin(command))
		return ("true\n");
	pid = fork();
	if (pid < 0)
	{
		printf("Error\n");
		exit(1);
	}
	if (pid == 0)
	{
		path = ft_strjoin("/bin/", command);
		if (execve(path, argv, envp) == -1)
			exit(1);
	}
	if (waitpid(pid, &status, 0) < 0)
	{
		printf("Error\n");
		exit(1);
	}
}

void	minishell(char *envp[])
{
	char *command;

	command = NULL;
	while (1)
	{
		if (signal(SIGINT, sigint_handler) == SIG_ERR)
		{
			printf("Error\n");
			exit(1);
		}
		if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		{
			printf("Error\n");
			exit(1);
		}
		command = readline("minishell> ");
		if (command == NULL)
			exit(1);
		else if (ft_strlen(command) > 0)
		{
			run_command(command, envp);
			add_history(command);
		}
		free(command);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc == 1)
		minishell(envp);
	return (0);
}