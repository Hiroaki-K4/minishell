#include "minishell.h"

void	init_global_state(t_global_state *state)
{
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->process_count = 0;
	state->pids = (int *)malloc(sizeof(int) * 10);
	if (state->pids == NULL)
		exit_with_error("initialize state error");
	state->redirect_fd = -1;
	state->file_fd = -1;
	state->here_delimiter = NULL;
	state->here_document = NULL;
}

void	refresh_global_state(t_global_state *state)
{
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->process_count = 0;
	free(state->pids);
	state->pids = (int *)malloc(sizeof(int) * 10);
	ft_bzero(state->pids, 10);
	if (state->pids == NULL)
		exit_with_error("initialize state error");
	state->redirect_fd = -1;
	state->file_fd = -1;
	free(state->here_delimiter);
	state->here_delimiter = NULL;
	free(state->here_document);
	state->here_document = NULL;
}

void	minishell(char *envp[])
{
	char				*input;
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;
	t_node				*ast;
	t_global_state		state;

	input = NULL;
	init_sigaction(&sa_sigint, &sa_sigquit);
	init_global_state(&state);
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
			exit(SUCCESS);  // TODO: when exitting, echo "exit"
		else if (ft_strlen(input) > 0)
		{
			ast = preprocess(input);
			if (ast == NULL)
				continue ;
			(void)envp;
			// execute(ast, envp, &state);
			add_history(input);
		}
		free(input);
		refresh_global_state(&state);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc == 1)
		minishell(envp);
	return (EXIT_SUCCESS);
}
