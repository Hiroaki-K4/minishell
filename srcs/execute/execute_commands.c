/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:55 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/18 00:06:38 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_command(char **argv, t_global_state *state)
{
	int			i;
	char		*path;
	t_redirect	*redirect;

	i = 0;
	while ((size_t)i < state->redirect_num)
	{
		redirect = state->redirects[i];
		if (redirect->file_fd)
			dup2(redirect->file_fd, redirect->redirect_fd);
		i++;
	}
	i = state->last_command_exit_status;
	if (is_builtin_command(argv, state->envs, &i)
		|| is_special_builtin_command(argv, state))
		exit(i);
	else
	{
		path = search(argv[0], state->envs);
		if (execve(path, argv, state->envs->content) == -1)
		{
			print_command_error(argv[0], "command not found");
			exit(127);
		}
	}
}

static void	execute_commands_parent(
	t_node *node,
	int pipes[2],
	t_global_state *state,
	pid_t pid
)
{
	size_t	i;

	close_parent_pipe(pipes, node, state);
	state->process_count++;
	state->pids[state->process_count - 1] = pid;
	i = 0;
	while (i < state->redirect_num)
	{
		if (state->redirects[i]->file_fd != -1)
			close(state->redirects[i]->file_fd);
		i++;
	}
	state->redirect_num = 0;
	if (node->is_furthest_right)
		wait_all_processes(state);
}

static int	execute_commands_with_argv(
	t_node *node,
	int pipes[2],
	t_global_state *state,
	char **argv
)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		exit_with_error("fork error");
	else if (pid == 0)
	{
		set_child_handlers(state);
		do_piping(pipes, node, state);
		execute_command(argv, state);
	}
	else
	{
		set_parent_handlers(state);
		execute_commands_parent(node, pipes, state, pid);
	}
	free_strings(argv);
	return (SUCCESS);
}

int	execute_commands(t_node *node, int pipes[2], t_global_state *state)
{
	char	**argv;

	argv = construct_argv(node->tokens, state);
	if (argv == NULL)
		return (FAIL);
	close_pipes(pipes, node, state);
	if (pipes == NULL
		&& is_special_builtin_command(argv, state))
	{
		free_strings(argv);
		return (SUCCESS);
	}
	return (execute_commands_with_argv(node, pipes, state, argv));
}
