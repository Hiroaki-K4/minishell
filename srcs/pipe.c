#include "minishell.h"

void	do_piping(int pipes[2], t_node *node, t_global_state *state)
{
	if (pipes == NULL)
		return ;
	if (node->is_furthest_left)
	{
		if (dup2(pipes[1], 1) < 0)
			exit_with_error("furthest left dup2 error");
		close(pipes[0]);
	}
	else if (node->is_furthest_right)
	{
		if (dup2(pipes[0], 0) < 0)
			exit_with_error("furthest right dup2 error");
	}
	else
	{
		if (dup2(state->old_pipes[0], 0) < 0)
			exit_with_error("internal1 dup2 error");
		if (dup2(pipes[1], 1) < 0)
			exit_with_error("internal2 dup2 error");
	}
}

void	close_pipes(int pipes[2], t_node *node, t_global_state *state)
{
	if (pipes == NULL)
		return ;
	if (node->is_furthest_left)
		;
	else if (node->is_furthest_right)
		close(pipes[1]);
	else
		close(state->old_pipes[1]);
}
