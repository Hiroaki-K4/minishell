/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_all_processes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:32 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 21:37:27 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	kill_all_processes(t_global_state *state)
{
	int	i;

	i = 0;
	while (state->pids[i])
	{
		kill(state->pids[i], SIGINT);
		i++;
	}
}

static void	handle_killed_by_signal(int status, t_global_state *state)
{
	int		signal;

	signal = WTERMSIG(status);
	if (signal == SIGINT)
		ft_putstr_fd("\n", 1);
	if (signal == SIGQUIT)
		ft_putendl_fd("Quit: 3", 1);
	state->last_command_exit_status = 128 + signal;
}

static int	wait_process(t_global_state *state)
{
	int		status;
	pid_t	finished_pid;

	finished_pid = waitpid(-1, &status, 0);
	if (WIFEXITED(status)
		&& finished_pid == state->pids[state->process_count - 1])
		state->last_command_exit_status = WEXITSTATUS(status);
	if (finished_pid < 0)
	{
		kill_all_processes(state);
		return (FAIL);
	}
	else
	{
		if (WIFSIGNALED(status))
			handle_killed_by_signal(status, state);
		return (SUCCESS);
	}
}

void	wait_all_processes(t_global_state *state)
{
	int	i;

	i = 0;
	while (i < state->process_count)
	{
		if (wait_process(state) == FAIL)
			continue ;
		i++;
	}
}
