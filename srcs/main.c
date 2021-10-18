#include "minishell.h"

void	sigint_handler()
{
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler()
{
	printf("sigquit\n");
	printf("\b\b  \b\b");
	// printf("sigquit_handler\n");
	// printf("\n");
}

void	minishell()
{
	char *command;

	command = NULL;
	while (1)
	{
		// printf("minishell");
		// line = readline("> ");
		// command = readline("minishell> ");
		// printf("len: %ld\n", strlen(command));
		if (signal(SIGINT, sigint_handler) == SIG_ERR)
		{
			printf("Error\n");
			exit(1);
		}
		if (signal(SIGQUIT, sigquit_handler) == SIG_ERR)
		{
			printf("Error\n");
			exit(1);
		}
		command = readline("minishell> ");
		printf("len: %ld\n", strlen(command));
		if (command == NULL)
		{
			printf("empty\n");
			exit(1);
		}
		else if (strlen(command) > 0)
			add_history(command);
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