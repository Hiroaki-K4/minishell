#include "minishell.h"

void	execute_command(char **argv, t_global_state *state)
{
	size_t		i;
	int			*status;
	char		*path;
	t_redirect	*redirect;

	i = 0;
	status = &(state->last_command_exit_status);
	while (i < state->redirect_num)
	{
		redirect = state->redirects[i];
		if (redirect->file_fd)
			dup2(redirect->file_fd, redirect->redirect_fd);
		i++;
	}
	if (is_builtin_command(argv, state->envs)
		|| is_special_builtin_command(argv, &(state->envs), status))
		exit(errno);
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

void	wait_all_processes(t_global_state *state)
{
	int		i;
	int		status;
	pid_t	finished_pid;

	i = 0;
	status = 0;
	set_sigaction(&state->sa_sigint, sigint_handler2);
	set_sigaction(&state->sa_sigquit, sigquit_handler2);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
	while (i < state->process_count)
	{
		finished_pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && finished_pid == state->pids[state->process_count - 1])
			state->last_command_exit_status = WEXITSTATUS(status);
		if (finished_pid < 0)
		{
			if (WIFSIGNALED(status))
			{
				int j = 0;
				while (state->pids[j])
				{
					kill(state->pids[j], SIGINT);
					j++;
				}
			}
			continue ;
		}
		i++;
	}
}

int	execute_commands(t_node *node, int pipes[2], t_global_state *state)
{
	size_t		i;
	pid_t		pid;
	char		**argv;

	set_sigaction(&state->sa_sigint, nop_handler);
	set_sigaction(&state->sa_sigquit, nop_handler);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
	argv = construct_argv(node->tokens, state);
	close_pipes(pipes, node, state);
	if (pipes == NULL && is_special_builtin_command(argv, &(state->envs), &(state->last_command_exit_status)))
		return (SUCCESS);
	pid = fork();
	if (pid < 0)
		exit_with_error("fork error");
	else if (pid == 0)
	{
		do_piping(pipes, node, state);
		execute_command(argv, state);
	}
	else
	{
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
	return (SUCCESS);
}

int	execute_pipe(t_node *ast, t_global_state *state)
{
	int		fildes[2];

	if (ast->lhs->attr == ND_PIPE)
	{
		execute_pipe(ast->lhs, state);
		if (pipe(fildes) == FAIL)
			exit_with_error("pipe error");
		execute_commands(ast->lhs->rhs, fildes, state);
		if (ast->is_top)
		{
			state->old_pipes[0] = fildes[0];
			state->old_pipes[1] = fildes[1];
			execute_commands(ast->rhs, fildes, state);
		}
		state->old_pipes[0] = fildes[0];
		state->old_pipes[1] = fildes[1];
	}
	else if (ast->lhs->attr == ND_COMMAND)
	{
		if (pipe(fildes) == FAIL)
			exit_with_error("pipe error");
		execute_commands(ast->lhs, fildes, state);
		if (ast->is_top)
			execute_commands(ast->rhs, fildes, state);
		state->old_pipes[0] = fildes[0];
		state->old_pipes[1] = fildes[1];
	}
	return (SUCCESS);
}

int	execute(t_node *ast, t_global_state *state)
{
	if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, state));
	return (execute_commands(ast, NULL, state));
}
