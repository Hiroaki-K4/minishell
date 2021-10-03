#include "minishell.h"





void	sigint_handler(int signum)
{
	printf("%d\n", signum);
	printf("ctl+c\n");
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
		line = readline("> ");
		if (line == NULL)
		{
			printf("\n");
			exit(1);
		}
		signal(SIGINT, sigint_handler);
		printf("line: %s\n", line);
	}
}

int	main(int argc, char *argv[])
{
	(void)argv;
	if (argc == 1)
		minishell();
	return (0);
}