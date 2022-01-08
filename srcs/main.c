#include "minishell.h"

void	init_global_state(t_global_state *state)
{
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->process_count = 0;
	state->pids = (int *)malloc(sizeof(int) * 10);
	if (state->pids == NULL)
		exit_with_error("initialize state error");
	state->last_command_exit_status = 0;
	state->redirects = NULL;
	state->redirect_num = 0;
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
	int i = 0;
	while (i < state->redirect_num)
	{
		free(state->redirects[i].here_delimiter);
		state->redirects[i].here_delimiter = NULL;
		free(state->redirects[i].here_document);
		state->redirects[i].here_document = NULL;
		i++;
	}
	state->redirect_num = 0;
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

int	init_envs(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	g_envs = (char **)malloc(sizeof(char *) * (i + 1));
	if (!g_envs)
		return (FAIL);
	i = 0;
	while (envp[i])
	{
		g_envs[i] = ft_strdup(envp[i]);
		i++;
	}
	g_envs[i] = NULL;
	return (SUCCESS);
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	init_envs(envp);
	if (argc == 1)
		minishell(envp);
	return (EXIT_SUCCESS);
}
