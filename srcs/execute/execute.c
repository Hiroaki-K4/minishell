#include "minishell.h"

void	execute_command(char **argv, t_global_state *state)
{
	int		i;
	char	*path;

	i = 0;
	while (i < state->redirect_num)
	{
		if (state->redirects[i]->file_fd)
			dup2(state->redirects[i]->file_fd, state->redirects[i]->redirect_fd);
		i++;
	}
	if (is_builtin_command(argv, state->envs, &(state->last_command_exit_status)))
		exit(errno);
	else
	{
		path = search(argv[0], state->envs);
		if (execve(path, argv, state->envs->content) == -1)
		{
			printf("minishell: %s: command not found\n", argv[0]);
			state->last_command_exit_status = 127;
			// printf("exit_status1: %d\n", state->last_command_exit_status);
			exit(errno);
			// exit(127);
		}
	}
}

void	wait_all_processes(t_global_state *state)
{
	int		i;
	int		status;
	pid_t	finished_pid;

	i = 0;
	while (i < state->process_count)
	{
		finished_pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			state->last_command_exit_status = WEXITSTATUS(status);
			printf("exit_status2: %d\n", state->last_command_exit_status);
		}
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
			continue;
		}
		i++;
	}
}

int	execute_commands(t_node *node, int pipes[2], t_global_state *state)
{
	int			i;
	pid_t		pid;
	char		**argv;

	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;
	init_sigaction2(&sa_sigint, &sa_sigquit);
	if (sigaction(SIGINT, &sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &sa_sigquit, NULL) < 0)
	{
		printf("Error\n");
		exit(FAIL);
	}

	argv = construct_argv(node->tokens, state);
	close_pipes(pipes, node, state);
	if (is_special_builtin_command(argv, &(state->envs), &(state->last_command_exit_status)))
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
		{
			wait_all_processes(state);
			// printf("[parent process]exit_status: %d\n", state->last_command_exit_status);
		}
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
