#include "minishell.h"

void	sigint_handler()
{
	// printf("\n");
	// printf("minishell> ");
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	// printf("ctl+c\n");
}

void	sigquit_handler()
{
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
		command = readline("minishell> ");
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
		// printf("minishell");
		// line = readline("minishell> ");
		// line = readline("> ");
		if (command == NULL)
			exit(1);
		else
			add_history(command);
		free(command);
		// printf("line: %s\n", line);
	}
}

int	main(int argc, char *argv[])
{
	(void)argv;
	if (argc == 1)
		minishell();
	return (0);
}