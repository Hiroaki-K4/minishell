#include "minishell.h"

void	minishell(char *envp[])
{
	char				*input;
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;
	t_node				*ast;

	input = NULL;
	init_sigaction(&sa_sigint, &sa_sigquit);
	while (TRUE)
	{
		if (sigaction(SIGINT, &sa_sigint, NULL) < 0
			|| sigaction(SIGQUIT, &sa_sigquit, NULL) < 0)
		{
			printf("Error\n");
			exit(FAIL);
		}
		input = readline("minishell> ");
		if (input == NULL)
			exit(FAIL);
		else if (ft_strlen(input) > 0)
		{
			ast = preprocess(input);
			if (ast == NULL)
				continue ;
			execute(ast, envp);
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
