#include "minishell.h"

void	sigint_handler()
{
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}


// TODO
// make launch function

void	run_command(char *command)
{
	pid_t	pid;
	char *path;

	pid = fork();
	if (pid < 0)
	{
		printf("Error\n");
		exit(1);
	}
	if (pid == 0)
	{
		path = strjoin("/bin/", command);
		printf(path);
	}
}

void	minishell()
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
		else if (strlen(command) > 0)
		{
			run_command(command);
			add_history(command);
		}
		free(command);
	}
}

int	main(int argc, char *argv[])
{
	(void)argv;
	if (argc == 1)
		minishell();
	return (0);
}