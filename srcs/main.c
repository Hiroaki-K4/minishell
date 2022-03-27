/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:49:08 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 17:08:27 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_global_state(t_global_state *state, char **envp)
{
	size_t	i;

	set_initial_handlers(state);
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->max_process_num = 100;
	state->max_redirect_num = 100;
	state->process_count = 0;
	state->pids = (pid_t *)malloc(sizeof(pid_t) * (state->max_process_num + 1));
	if (state->pids == NULL)
		exit_with_error("initialize state error");
	state->last_command_exit_status = 0;
	state->redirects = (t_redirect **)malloc(sizeof(t_redirect *)
			* (state->max_redirect_num + 1));
	if (state->redirects == NULL)
		exit_with_error("initialize state error");
	i = 0;
	while (i < state->max_redirect_num + 1)
	{
		state->redirects[i] = NULL;
		i++;
	}
	state->redirect_num = 0;
	if (init_envs(&(state->envs), envp) == FAIL)
		printf("init_env error\n");
}

static void	refresh_global_state(t_global_state *state)
{
	size_t	i;

	set_initial_handlers(state);
	state->old_pipes[0] = 0;
	state->old_pipes[1] = 0;
	state->process_count = 0;
	ft_bzero(state->pids, sizeof(pid_t) * (state->max_process_num + 1));
	i = -1;
	while (++i < state->max_redirect_num + 1)
	{
		if (state->redirects[i])
		{
			free(state->redirects[i]->here_delimiter);
			state->redirects[i]->here_delimiter = NULL;
			free(state->redirects[i]->here_document);
			state->redirects[i]->here_document = NULL;
			free(state->redirects[i]);
			state->redirects[i] = NULL;
		}
	}
	state->redirect_num = 0;
}

static void	process_input(char *input, t_global_state *state, int is_debug_mode)
{
	t_node	*ast;

	add_history(input);
	ast = preprocess(input, state, is_debug_mode);
	if (ast == NULL)
		return ;
	if (!is_debug_mode)
		execute(ast, state);
	free_ast(ast);
}

static void	minishell(char *envp[], int is_debug_mode)
{
	char				*input;
	t_global_state		state;

	input = NULL;
	init_global_state(&state, envp);
	while (TRUE)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			ft_putstr_fd("exit\n", 1);
			exit(SUCCESS);
		}
		else if (ft_strlen(input) > 0)
			process_input(input, &state, is_debug_mode);
		free(input);
		refresh_global_state(&state);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc == 1)
		minishell(envp, FALSE);
	if (argc == 2 && !ft_strncmp(argv[1], "tokenize", ft_strlen(argv[1]) + 1))
		minishell(envp, TRUE);
	return (EXIT_SUCCESS);
}
