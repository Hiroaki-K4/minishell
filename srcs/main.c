#include "minishell.h"





void	sigint_handler()
{
	printf("\n");
	printf("minishell>");
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
	char *line;

	line = NULL;
	while (1)
	{
		printf("minishell");
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
		line = readline("> ");
		if (line == NULL)
			exit(1);
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