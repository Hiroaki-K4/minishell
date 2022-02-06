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
		|| is_special_builtin_command(argv, &(state->envs), &i))
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

static int	wait_process(t_global_state *state)
{
	size_t	i;
	int		status;
	int		signal;
	pid_t	finished_pid;

	finished_pid = waitpid(-1, &status, 0);
	if (WIFEXITED(status)
		&& finished_pid == state->pids[state->process_count - 1])
		state->last_command_exit_status = WEXITSTATUS(status);
	if (finished_pid < 0)
	{
		i = 0;
		while (state->pids[i])
		{
			kill(state->pids[i], SIGINT);
			i++;
		}
		return (FAIL);
	}
	else
	{
		if (WIFSIGNALED(status))
		{
			signal = WTERMSIG(status);
			if (signal == SIGINT)
				ft_putstr_fd("\n", 1);
			if (signal == SIGQUIT)
				ft_putendl_fd("Quit: 3", 1);
		}
		return (SUCCESS);
	}
}

static void	wait_all_processes(t_global_state *state)
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

int	execute_commands(t_node *node, int pipes[2], t_global_state *state)
{
	pid_t	pid;
	char	**argv;

	argv = construct_argv(node->tokens, state);
	close_pipes(pipes, node, state);
	if (pipes == NULL
		&& is_special_builtin_command(
			argv, &(state->envs), &(state->last_command_exit_status)))
		return (SUCCESS);
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
	return (SUCCESS);
}
