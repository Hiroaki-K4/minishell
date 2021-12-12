#include "minishell.h"

int	check_builtin(char *command)
{
	if (!ft_strncmp(command, "echo", 5))
		return (1);
	else if (!ft_strncmp(command, "exit", 5))
		exit(0);
	return (0);
}

int	run_command(char *command, char *envp[])
{
	int		status;
	pid_t	pid;
	char	*path;
	char	**argv;

	argv = NULL;
	argv[0] = command;
	if (check_builtin(command) == 1)
	{
		printf("builtin\n");
		return (0);
	}
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
	return (0);
}

void	init_sigaction(struct sigaction	*sa_sigint,
	struct sigaction *sa_sigquit)
{
	ft_bzero(sa_sigint, sizeof(*sa_sigint));
	ft_bzero(sa_sigquit, sizeof(*sa_sigquit));
	sa_sigint->sa_handler = sigint_handler;
	sa_sigquit->sa_handler = sigquit_handler;
}

void	minishell(char *envp[])
{
	char				*input;
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	(void)envp;
	input = NULL;
	init_sigaction(&sa_sigint, &sa_sigquit);
	while (TRUE)
	{
		if (sigaction(SIGINT, &sa_sigint, NULL) < 0)
		{
			printf("Error\n");
			exit(FAIL);
		}
		if (sigaction(SIGQUIT, &sa_sigquit, NULL) < 0)
		{
			printf("Error\n");
			exit(FAIL);
		}
		input = readline("minishell> ");
		if (input == NULL)
			exit(FAIL);
		else if (ft_strlen(input) > 0)
		{
			if (preprocess(input) == FAIL)
				continue ;
			// run_command(input, envp);
			add_history(input);
		}
		free(input);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc == 1)
		minishell(envp);
	return (EXIT_SUCCESS);
}
