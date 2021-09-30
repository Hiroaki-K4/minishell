#include "minishell.h"





void	minishell()
{
	printf("minishell\n");
}

int	main(int argc, char *argv[])
{
	(void)argv;
	if (argc == 1)
		minishell();
	return (0);
}