#include "minishell.h"





void	minishell()
{
	char *line;

	line = NULL;
	while (1)
	{
		printf("minishell");
		line = readline("> ");
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