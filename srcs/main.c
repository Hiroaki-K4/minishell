#include "minishell.h"

void	init_global_state(t_global_state *state, char **envp)
{
	set_sigaction(&state->sa_sigint, sigint_handler);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
	{
		printf("Error\n");
		exit(FAIL);
	}
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->process_count = 0;
	state->pids = (int *)malloc(sizeof(int) * 10);  // TODO: hard coded!!!
	if (state->pids == NULL)
		exit_with_error("initialize state error");
	state->last_command_exit_status = 0;
	state->redirects = (t_redirect **)malloc(sizeof(t_redirect *) * (10 + 1));  // TODO: hard coded!!!
	if (state->redirects == NULL)
		exit_with_error("initialize state error");
	int i = 0;
	while (i < 11)
	{
		state->redirects[i] = NULL;
		i++;
	}
	state->redirect_num = 0;
	if (init_envs(&(state->envs), envp) == FAIL)
		printf("init_env error\n");
}

void	refresh_global_state(t_global_state *state)
{
	int	i;

	set_sigaction(&state->sa_sigint, sigint_handler);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
	{
		printf("Error\n");
		exit(FAIL);
	}
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->process_count = 0;
	free(state->pids);
	state->pids = (int *)malloc(sizeof(int) * 10);
	ft_bzero(state->pids, 10);
	if (state->pids == NULL)
		exit_with_error("initialize state error");
	i = 0;
	while (i < state->redirect_num)
	{
		free(state->redirects[i]->here_delimiter);
		state->redirects[i]->here_delimiter = NULL;
		free(state->redirects[i]->here_document);
		state->redirects[i]->here_document = NULL;
		i++;
	}
	state->redirect_num = 0;
}

void	minishell(char *envp[], int tokenize)
{
	char				*input;
	t_node				*ast;
	t_global_state		state;

	input = NULL;
	init_global_state(&state, envp);
	while (TRUE)
	{
		input = readline("minishell> ");
		if (input == NULL)
			exit(SUCCESS);  // TODO: when exitting, echo "exit"
		else if (ft_strlen(input) > 0)
		{
			ast = preprocess(input, &state, tokenize);
			if (ast == NULL)
				continue ;
			(void)envp;
			if (tokenize == FALSE)
				execute(ast, &state);
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
		minishell(envp, FALSE);
	// We must delete this if-sentence when submitting the code.
	if (argc == 2 && ft_strncmp(argv[1], "tokenize", ft_strlen(argv[1]) + 1) == 0)
		minishell(envp, TRUE);
	return (EXIT_SUCCESS);
}
